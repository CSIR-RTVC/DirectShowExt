/**
LICENSE: Software License Agreement(BSD License)

Copyright(c) 2017, CSIR
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and / or other materials provided with the distribution.
* Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===========================================================================
*/
#pragma once
#include <string>

#pragma warning(push)     // disable for this header only
#pragma warning(disable:4312) 
// DirectShow
#include <streams.h>
#pragma warning(pop)      // restore original warning level
#include <DirectShowExt/DirectShowMediaFormats.h>

/**
 * @brief appends parameter sets to format block.
 * sSps and sPps must contain the sequence and picture parameter sets each prepended by a start code
 */
bool appendParameterSetsToFormatBlock(CMediaType *pMediaType, const std::string& sSps, const std::string& sPps)
{
  if (!pMediaType) return false;
  if (pMediaType->formattype != FORMAT_VideoInfo) return false;
  if (pMediaType->subtype != MEDIASUBTYPE_VPP_H264) return false;

  VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)pMediaType->pbFormat;
  BITMAPINFOHEADER* pBi = &(pVih->bmiHeader);
  // Store SPS and PPS in media format header
  int nCurrentFormatBlockSize = pMediaType->cbFormat;

  // old size + one int to store size of SPS/PPS + SPS/PPS/prepended by start codes
  int iAdditionalLength = sizeof(int) + sSps.length() + sPps.length();
  int nNewSize = nCurrentFormatBlockSize + iAdditionalLength;
  pMediaType->ReallocFormatBuffer(nNewSize);
  pMediaType->cbFormat = nNewSize;
  BYTE* pFormat = pMediaType->Format();
  BYTE* pStartPos = &(pFormat[nCurrentFormatBlockSize]);
  // copy SPS and PPS: both need to be nonempty!
  if (!sSps.empty() && !sPps.empty())
  {
    memcpy(pStartPos, sSps.c_str(), sSps.length());
    pStartPos += sSps.length();
    memcpy(pStartPos, sPps.c_str(), sPps.length());
    pStartPos += sPps.length();
  }
  // Copy additional header size
  memcpy(pStartPos, &iAdditionalLength, sizeof(int));
  return true;
}

/**
 * @brief extracts parameter sets from format block.
 * Parameter sets will contain a start code.
 */
bool extractParameterSetsFromFormatBlock(const CMediaType *pMediaType, std::string& sSps, std::string& sPps)
{
  if (!pMediaType) return false;
  if (pMediaType->formattype != FORMAT_VideoInfo) return false;
  if (pMediaType->subtype != MEDIASUBTYPE_VPP_H264) return false;

  int nSize = pMediaType->cbFormat;
  BYTE* pFormat = pMediaType->Format();
  unsigned uiAdditionalSize(0);
  memcpy(&uiAdditionalSize, pFormat + nSize - sizeof(int), sizeof(int));

  if (uiAdditionalSize > 0)
  {
    unsigned char startCode[4] = { 0, 0, 0, 1 };
    char* pBuffer = new char[uiAdditionalSize];
    memcpy(pBuffer, pFormat + (nSize - uiAdditionalSize), uiAdditionalSize);
    // check for first start code
    if (memcmp(startCode, pBuffer, 4) == 0)
    {
      int index = 0;
      // adjust to compensate for start code
      for (size_t i = 4; i < uiAdditionalSize - 8; ++i) // skip 1st start code and since start code is 4 bytes long don't cmp past the end
      {
        // search for second start code
        if (memcmp(startCode, &pBuffer[i], 4) == 0)
        {
          index = i;
          break;
        }
      }

      // if we found the second start code
      if (index != 0)
      {
        sSps = std::string(pBuffer, index);
        sPps = std::string(pBuffer + index, uiAdditionalSize - index - sizeof(int));
        return true;
      }
    }
  }
  return false;
}

