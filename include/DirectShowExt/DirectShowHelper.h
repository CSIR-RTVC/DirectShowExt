/** @file

MODULE				: DirectShow

FILE NAME			: DirectShowHelper.h

DESCRIPTION			: DirectShow helper methods based on SDK examples

LICENSE: Software License Agreement (BSD License)

Copyright (c) 2008 - 2017, CSIR
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the CSIR nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

===========================================================================
*/
#pragma once

#pragma warning(push)     // disable for this header only
#pragma warning(disable:4312)
// DirectShow
#include <streams.h>
#pragma warning(pop)      // restore original warning level

#include <vector>
#include <string>

/*! \var typedef unsigned int UINT32
\brief A type definition for a .
Details.
*/
typedef std::vector<std::string> FILTER_NAME_LIST;
typedef std::vector<std::pair<std::string, std::string>> FILTER_NAME_LIST_VIDEODEVICE;

/**
* \ingroup DirectShowLib
* Helper class that provides static DirectShow related helper methods for generic DirectShow functionality
*/
class CDirectShowHelper
{
public:
  CDirectShowHelper();

  /// Find an connected pin on a filter.
  /// This too is stolen from the DX9 SDK.
  static HRESULT GetConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

  /// Find an unconnected pin on a filter.
  /// This too is stolen from the DX9 SDK.
  static HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, const GUID* pMajorType = NULL, int iPinsToSkip = 0);

  /// Connect two filters together with the filter graph manager.
  /// Stolen from the DX9 SDK.
  /// This is the base version.
  static HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest);

  /// Connect two filters together with the filter graph manager.
  /// Again, stolen from the DX9 SDK.
  /// This is an overloaded version.
  static HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, const GUID* pMajorType = NULL);

  static HRESULT ConnectCaptureSourceFilters(ICaptureGraphBuilder2* pGraphBuilder, IBaseFilter *pSrc, IBaseFilter *pDest);

  /// Helper function to retrieve filters of a certain type using wide string
  static HRESULT GetFilterByCategory(IBaseFilter** gottaFilter, LPCWSTR matchName, GUID Category);

  /// Helper function to retrieve filters of a certain type using STL string.
  static HRESULT GetFilterByCategory(IBaseFilter** gottaFilter, std::string sMatchName, GUID Category);

  /// This code was also brazenly stolen from the DX9 SDK.
  /// Pass it a file name in wszPath, and it will save the filter graph to that file.
  static HRESULT SaveGraphFile(IGraphBuilder *pGraph, const WCHAR *wszPath);

  /// Helper method to add filter to graph using GUID.
  /// Stolen from the DX9 SDK.
  static HRESULT AddFilterByCLSID(IGraphBuilder *pGraph, const GUID& clsid, LPCWSTR wszName, IBaseFilter **ppF);
  //static HRESULT AddFilterByCLSID(IGraphBuilder *pGraph, const GUID& clsid, std::string sName, IBaseFilter **ppF);

  /// Helper method to add filter to graph using category GUID and name.
  static HRESULT AddFilterByCategoryAndName(IGraphBuilder *pGraph, const GUID& category, LPCWSTR wszName, IBaseFilter **ppF);
  /// Helper method to add filter to graph using category GUID and name.
  static HRESULT AddFilterByCategoryAndName(IGraphBuilder *pGraph, const GUID& category, std::string sName, IBaseFilter **ppF);

  /// Show the property pages for a filter.
  /// This is stolen from the DX9 SDK.
  HRESULT ShowFilterPropertyPages(IBaseFilter *pFilter);

  /// Retrieves filter names in a vector filtered by category.
  static HRESULT GetFilterNamesByCategory(GUID Category, FILTER_NAME_LIST& vList);
  /// Retrieves video device filter names and device path in a vector.
  static HRESULT GetVideoDeviceFilterInfo(FILTER_NAME_LIST_VIDEODEVICE& vFilters);
  /// Retrieves video device filter names in a vector.
  static HRESULT GetVideoInputs(FILTER_NAME_LIST& vList);
  /// Retrieves video device filter names in a vector.
  static HRESULT GetVideoInputs(FILTER_NAME_LIST_VIDEODEVICE& vList);
  /// Retrieves audio device filter names in a vector.
  static HRESULT GetAudioInputs(FILTER_NAME_LIST& vList);

  /// Helper method to retrieve pin from filter.
  static HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

  static HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);

  static void RemoveGraphFromRot(DWORD pdwRegister);

  static HRESULT Render(IGraphBuilder *pGraph, IBaseFilter *pFilter);

  static std::string GetTvFormatString(long lFormat);
  /**
  * @brief Get the first upstream or downstream filter
  * @param[in] pFilter Pointer to the starting filter
  * @param[in] Dir Direction to search (upstream or downstream)
  * @param[out] ppNext Receives a pointer to the next filter
  */
  static HRESULT GetNextFilter(IBaseFilter *pFilter, PIN_DIRECTION Dir, IBaseFilter **ppNext);
  /**
  * @brief Get the filter connected to the pin
  * @param[in] pPin Pointer to the local pin
  * @param[out] ppNext Receives a pointer to the next filter
  */
  static HRESULT GetFilterConnectedToPin(IPin *pPin, IBaseFilter **ppNext);
  /**
  * @brief Gets the first upstream or downstream filter that implements the specified COM interface.
  * The calling code must release the interface.
  * and gets a pointer to the interface.
  * @param[in] pFilter Pointer to the starting filter
  * @param[in] Dir Direction to search (upstream or downstream)
  * @param[in] riid The interface to search for
  * @param[out] ppUnk Receives a pointer to the interface if the method is successful
  */
  static HRESULT FindFirstInterface(IBaseFilter *pFilter, PIN_DIRECTION Dir, REFIID riid, void **ppUnk);
  /**
  * @brief Gets the first upstream or downstream filter that implements the specified COM interface
  * The calling code must release the filter if the method is successful.
  * @param[in] pFilter Pointer to the starting filter
  * @param[in] Dir Direction to search (upstream or downstream)
  * @param[in] riid The interface to search for
  * @param[out] pResult Receives a pointer to the first filter that implements the specified interface if the method is successful
  */
  static HRESULT FindFirstFilterWithInterface(IBaseFilter *pFilter, PIN_DIRECTION Dir, REFIID riid, IBaseFilter** pResult);
  /**
   * @brief
   */
  static HRESULT GetCrossBarInfo(const std::string& sVideoCaptureDevice, std::vector<std::string>& vInputPins, std::vector<std::string>& vOutputPins);
  /**
   * @brief gets the width and height of the file based on DirectShow media types
   */
  static HRESULT GetFileMediaInfo(const std::string& sFile, double& dHeight, double& dWidth, GUID& MediaSubtype);
  /**
   * @brief gets the frame rate of the file based on DirectShow media types
   */
  static HRESULT GetFileFramerate(const std::string& sFile, double& dFramerate, std::string& error);
  /**
   * @brief gets the width and height of the file based on DirectShow media types
   */
  static HRESULT GetFileVideoInfo(const std::string& sFile, int& iHeight, int& iWidth, GUID& MediaSubtype);
  /**
   * @brief determines if the specified file has video and audio outputs.
   *
   * Very simple check using the format_type. Checking for FORMAT_VideoInfo, FORMAT_VideoInfo2 and FORMAT_WaveFormatEx
   */
  static HRESULT FileHasVideoAndAudio(const std::string& sFile, bool& bHasVideo, bool& bHasAudio);
  static HRESULT VerifyVideoDevice(const std::string& sVideoSource, const std::string& sVideoCrossbarSource = "");
  static HRESULT VerifyAudioDevice(const std::string& sAudioSource);
  static HRESULT GetVideoDeviceCapabilityList(const std::string& sVideoSource, std::vector<std::string>& sVideoColorSpace,
    std::vector<double>& dVideoFrameRate, std::vector<std::string>& sVideoResolution);
  static bool CanSupplyType(IPin* pPin, const GUID* pMajorType);
  static HRESULT FindStreamSource(IBaseFilter* pFilter, const GUID* pMajorType, IPin** ppPin);
private:
  static bool MapClassIDToString(const GUID* pGuid, std::string& sClassIdToString);
};
