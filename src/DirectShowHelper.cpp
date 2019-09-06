/** @file

MODULE				: DirectShow

FILE NAME			: DirectShowHelper.cpp

DESCRIPTION			:

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
#include "stdafx.h"
#include "DirectShowHelper.h"
#include <dvdmedia.h>
#include "ComTypeDefs.h"
#include "StringUtil.h"
#include "Crossbar.h"
#include <set>

CDirectShowHelper::CDirectShowHelper()
{

}

HRESULT CDirectShowHelper::GetConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
  *ppPin = 0;
  IEnumPins *pEnum = 0;
  IPin *pPin = 0;
  HRESULT hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr))
  {
    return hr;
  }
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    PIN_DIRECTION ThisPinDir;
    pPin->QueryDirection(&ThisPinDir);
    if (ThisPinDir == PinDir)
    {
      IPin *pTmp = 0;
      hr = pPin->ConnectedTo(&pTmp);
      if (SUCCEEDED(hr))  // Already connected, the pin we want.
      {
        pEnum->Release();
        *ppPin = pPin;
        return S_OK;
      }
      else  // Unconnected, this is not the pin we want.
      {
        pTmp->Release();
      }
    }
    pPin->Release();
  }
  pEnum->Release();
  // Did not find a matching pin.
  return E_FAIL;
}

HRESULT CDirectShowHelper::GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, const GUID* pMajorType, int iPinsToSkip)
{
  *ppPin = 0;
  IEnumPins *pEnum = 0;
  IPin *pPin = 0;
  HRESULT hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr))
  {
    return hr;
  }
  int iSkippedPins = 0;
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    PIN_DIRECTION ThisPinDir;
    pPin->QueryDirection(&ThisPinDir);
    if (ThisPinDir == PinDir)
    {
      IPin *pTmp = 0;
      hr = pPin->ConnectedTo(&pTmp);
      if (SUCCEEDED(hr))  // Already connected, not the pin we want.
      {
        pTmp->Release();
      }
      else  // Unconnected, this is the pin we want.
      {
        if (iSkippedPins < iPinsToSkip )
        {
          ++iSkippedPins;
        }
        else
        {
          if ((pMajorType == NULL) ||
            CDirectShowHelper::CanSupplyType(pPin, pMajorType))
          {
            pEnum->Release();
            *ppPin = pPin;
            return S_OK;
          }
        }
      }
    }
    pPin->Release();
  }
  pEnum->Release();
  // Did not find a matching pin.
  return E_FAIL;
}

HRESULT CDirectShowHelper::ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest)
{
  if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
  {
    return E_POINTER;
  }

  // Find an input pin on the downstream filter.
  IPin *pIn = 0;
  HRESULT hr = CDirectShowHelper::GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
  if (FAILED(hr))
  {
    return hr;
  }
  // Try to connect them.
  hr = pGraph->Connect(pOut, pIn);
  pIn->Release();
  return hr;
}

HRESULT CDirectShowHelper::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, const GUID* pMajorType)
{
  if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
  {
    return E_POINTER;
  }

  // iterate in case there are other unconnected pins 
  HRESULT hr;
  int iPinsToSkip = 0;
  do
  {
    // Find an output pin on the first filter.
    IPin *pOut = 0;
    hr = CDirectShowHelper::GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut, pMajorType, iPinsToSkip);
    if (FAILED(hr))
    {
      break;
    }
    hr = ConnectFilters(pGraph, pOut, pDest);
    pOut->Release();
    if (SUCCEEDED(hr))
    {
      break;
    }
    ++iPinsToSkip;
  } while (false);

  return hr;
}

//HRESULT CDirectShowHelper::GetFilterByCategory( IBaseFilter** gottaFilter, wchar_t* matchName, GUID Category )
HRESULT CDirectShowHelper::GetFilterByCategory(IBaseFilter** gottaFilter, LPCWSTR matchName, GUID Category)
{
  BOOL done = false;

  // Create the System Device Enumerator.
  ICreateDevEnum *pSysDevEnum = NULL;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
    IID_ICreateDevEnum, (void **)&pSysDevEnum);
  if (FAILED(hr))
  {
    return hr;
  }

  // Obtain a class enumerator for the audio input category.
  IEnumMoniker *pEnumCat = NULL;
  hr = pSysDevEnum->CreateClassEnumerator(Category, &pEnumCat, 0);

  if (hr == S_OK)
  {
    // Enumerate the monikers.
    IMoniker *pMoniker = NULL;
    ULONG cFetched;
    while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
    {
      // Bind the first moniker to an object
      IPropertyBag *pPropBag;
      hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
        (void **)&pPropBag);
      if (SUCCEEDED(hr))
      {
        // To retrieve the filter's friendly name, do the following:
        VARIANT varName;
        VariantInit(&varName);
        hr = pPropBag->Read(L"FriendlyName", &varName, 0);
        if (SUCCEEDED(hr))
        {
          //wprintf(L"Testing Device: %s\n", varName.bstrVal);

          // Do a comparison, find out if it's the right one
          if (wcsncmp(varName.bstrVal, matchName,
            wcslen(matchName)) == 0) {

            // We found it, so send it back to the caller
            hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)gottaFilter);
            done = true;
          }
        }
        VariantClear(&varName);
        pPropBag->Release();
      }
      pMoniker->Release();
    }
    pEnumCat->Release();
  }
  pSysDevEnum->Release();
  if (done) {
    return hr;	// found it, return native error
  }
  else {
    return VFW_E_NOT_FOUND;	// didn't find it error
  }
}

HRESULT CDirectShowHelper::GetFilterByCategory(IBaseFilter** gottaFilter, std::string sMatchName, GUID Category)
{
  std::wstring wsName = stringToWideString(sMatchName);
  HRESULT hr = CDirectShowHelper::GetFilterByCategory(gottaFilter, wsName.c_str(), Category);
  return hr;
}

HRESULT CDirectShowHelper::SaveGraphFile(IGraphBuilder *pGraph, const WCHAR *wszPath)
{
  const WCHAR wszStreamName[] = L"ActiveMovieGraph";
  HRESULT hr;
  IStorage *pStorage = NULL;

  // First, create a document file which will hold the GRF file
  hr = StgCreateDocfile(
    wszPath,
    STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0, &pStorage);
  if (FAILED(hr))
  {
    return hr;
  }

  // Next, create a stream to store.
  IStream *pStream;
  hr = pStorage->CreateStream(
    wszStreamName,
    STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
    0, 0, &pStream);
  if (FAILED(hr))
  {
    pStorage->Release();
    return hr;
  }

  // The IPersistStream converts a stream into a persistent object.
  IPersistStream *pPersist = NULL;
  pGraph->QueryInterface(IID_IPersistStream, reinterpret_cast<void**>(&pPersist));
  hr = pPersist->Save(pStream, TRUE);
  pStream->Release();
  pPersist->Release();
  if (SUCCEEDED(hr))
  {
    hr = pStorage->Commit(STGC_DEFAULT);
  }
  pStorage->Release();
  return hr;
}

HRESULT CDirectShowHelper::AddFilterByCLSID(IGraphBuilder *pGraph, const GUID& clsid, LPCWSTR wszName, IBaseFilter **ppF)
{
  if (!pGraph || !ppF) return E_POINTER;
  *ppF = 0;
  IBaseFilter *pF = 0;
  HRESULT hr = CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER,
    IID_IBaseFilter, reinterpret_cast<void**>(&pF));
  if (SUCCEEDED(hr))
  {
    hr = pGraph->AddFilter(pF, wszName);
    if (SUCCEEDED(hr))
      *ppF = pF;
    else
      pF->Release();
  }
  return hr;
}

HRESULT CDirectShowHelper::ShowFilterPropertyPages(IBaseFilter *pFilter)
{
  /* Obtain the filter's IBaseFilter interface. (Not shown) */
  ISpecifyPropertyPages *pProp;
  HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
  if (SUCCEEDED(hr))
  {
    // Get the filter's name and IUnknown pointer.
    FILTER_INFO FilterInfo;
    hr = pFilter->QueryFilterInfo(&FilterInfo);
    IUnknown *pFilterUnk;
    pFilter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);

    // Show the page. 
    CAUUID caGUID;
    pProp->GetPages(&caGUID);
    pProp->Release();
    OleCreatePropertyFrame(
      NULL,                   // Parent window
      0, 0,                   // Reserved
      FilterInfo.achName,     // Caption for the dialog box
      1,                      // Number of objects (just the filter)
      &pFilterUnk,            // Array of object pointers. 
      caGUID.cElems,          // Number of property pages
      caGUID.pElems,          // Array of property page CLSIDs
      0,                      // Locale identifier
      0, NULL                 // Reserved
      );

    // Clean up.
    pFilterUnk->Release();
    FilterInfo.pGraph->Release();
    CoTaskMemFree(caGUID.pElems);
  }
  return hr;
}

HRESULT CDirectShowHelper::GetFilterNamesByCategory(GUID Category, FILTER_NAME_LIST& vFilters)
{
  BOOL done = false;
  // Create the System Device Enumerator.
  ICreateDevEnum *pSysDevEnum = NULL;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
    IID_ICreateDevEnum, (void **)&pSysDevEnum);
  if (FAILED(hr))
  {
    return hr;
  }

  // Obtain a class enumerator for the audio input category.
  IEnumMoniker *pEnumCat = NULL;
  hr = pSysDevEnum->CreateClassEnumerator(Category, &pEnumCat, 0);

  if (hr == S_OK)
  {
    // Enumerate the monikers.
    IMoniker *pMoniker = NULL;
    ULONG cFetched;
    while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
    {
      // Bind the first moniker to an object
      IPropertyBag *pPropBag;
      hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
        (void **)&pPropBag);
      if (SUCCEEDED(hr))
      {
        // To retrieve the filter's friendly name, do the following:
        VARIANT varName;
        VariantInit(&varName);
        hr = pPropBag->Read(L"FriendlyName", &varName, 0);
        if (SUCCEEDED(hr))
        {
          LPCWSTR mychar; // narrow string
          mychar = (LPCWSTR)(varName.bstrVal);
          vFilters.push_back(wideStringToString(mychar));
        }
        VariantClear(&varName);
        pPropBag->Release();
      }
      pMoniker->Release();
    }
    pEnumCat->Release();
  }
  pSysDevEnum->Release();
  return S_OK;
}

HRESULT CDirectShowHelper::GetVideoDeviceFilterInfo(FILTER_NAME_LIST_VIDEODEVICE& vFilters)
{
  BOOL done = false;
  // Create the System Device Enumerator.
  ICreateDevEnum *pSysDevEnum = NULL;
  HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
    IID_ICreateDevEnum, (void **)&pSysDevEnum);
  if (FAILED(hr))
  {
    return hr;
  }

  // Obtain a class enumerator for the audio input category.
  IEnumMoniker *pEnumCat = NULL;
  hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

  if (hr == S_OK)
  {
    // Enumerate the monikers.
    IMoniker *pMoniker = NULL;
    ULONG cFetched;
    while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
    {
      // Bind the first moniker to an object
      IPropertyBag *pPropBag;
      hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
        (void **)&pPropBag);
      if (SUCCEEDED(hr))
      {
        // To retrieve the filter's friendly name, do the following:
        VARIANT varNameFriendly;
        VariantInit(&varNameFriendly);
        hr = pPropBag->Read(L"FriendlyName", &varNameFriendly, 0);
        if (SUCCEEDED(hr))
        {
          LPCWSTR mycharFriendly; // narrow string
          mycharFriendly = (LPCWSTR)(varNameFriendly.bstrVal);
          VARIANT varNameDevice;
          VariantInit(&varNameDevice);
          hr = pPropBag->Read(L"DevicePath", &varNameDevice, 0);
          if (SUCCEEDED(hr))
          {
            LPCWSTR mycharDevice; // narrow string
            mycharDevice = (LPCWSTR)(varNameDevice.bstrVal);
            //TODO
            vFilters.push_back(std::make_pair(wideStringToString(mycharFriendly), wideStringToString(mycharDevice)));
            //USES_CONVERSION;
            //char* szTempBuffer = W2A(mychar);
            //szTempBuffer  = W2A(mychar);
            //vFilters.push_back(std::string(szTempBuffer));
          }
          // In the case that the device does not have a DevicePath we store only the friendly name
          else
          {
            vFilters.push_back(std::make_pair(wideStringToString(mycharFriendly), std::string("")));
          }
          VariantClear(&varNameDevice);
        }
        VariantClear(&varNameFriendly);
        pPropBag->Release();
      }
      pMoniker->Release();
    }
    pEnumCat->Release();
  }
  pSysDevEnum->Release();
  return S_OK;
}


HRESULT CDirectShowHelper::GetVideoInputs(FILTER_NAME_LIST& vList)
{
  return GetFilterNamesByCategory(CLSID_VideoInputDeviceCategory, vList);
}

HRESULT CDirectShowHelper::GetVideoInputs(FILTER_NAME_LIST_VIDEODEVICE& vList)
{
  return GetVideoDeviceFilterInfo(vList);
}

HRESULT CDirectShowHelper::GetAudioInputs(FILTER_NAME_LIST& vList)
{
  return GetFilterNamesByCategory(CLSID_AudioInputDeviceCategory, vList);
}

HRESULT CDirectShowHelper::AddFilterByCategoryAndName(IGraphBuilder *pGraph, const GUID& category, LPCWSTR wszName, IBaseFilter **ppF)
{
  HRESULT hr = CDirectShowHelper::GetFilterByCategory(ppF, wszName, category);
  if (SUCCEEDED(hr))
  {
    hr = pGraph->AddFilter(*ppF, wszName);
  }
  return hr;
}

HRESULT CDirectShowHelper::AddFilterByCategoryAndName(IGraphBuilder *pGraph, const GUID& category, std::string sName, IBaseFilter **ppF)
{
  std::wstring wsName = stringToWideString(sName);
  HRESULT hr = CDirectShowHelper::AddFilterByCategoryAndName(pGraph, category, wsName.c_str(), ppF);
  return hr;
}

HRESULT CDirectShowHelper::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
  IEnumPins  *pEnum = NULL;
  IPin       *pPin = NULL;
  HRESULT    hr;

  if (ppPin == NULL)
  {
    return E_POINTER;
  }

  hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr))
  {
    return hr;
  }
  while (pEnum->Next(1, &pPin, 0) == S_OK)
  {
    PIN_DIRECTION PinDirThis;
    hr = pPin->QueryDirection(&PinDirThis);
    if (FAILED(hr))
    {
      pPin->Release();
      pEnum->Release();
      return hr;
    }
    if (PinDir == PinDirThis)
    {
      // Found a match. Return the IPin pointer to the caller.
      *ppPin = pPin;
      pEnum->Release();
      return S_OK;
    }
    // Release the pin for the next time through the loop.
    pPin->Release();
  }
  // No more pins. We did not find a match.
  pEnum->Release();
  return E_FAIL;
}

HRESULT CDirectShowHelper::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
  IMoniker * pMoniker;
  IRunningObjectTable *pROT;
  WCHAR wsz[128];
  HRESULT hr;

  if (FAILED(GetRunningObjectTable(0, &pROT)))
    return E_FAIL;

  wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
  wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());

  hr = CreateItemMoniker(L"!", wsz, &pMoniker);
  if (SUCCEEDED(hr))
  {
    hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
    pMoniker->Release();
  }

  pROT->Release();
  return hr;
}

void CDirectShowHelper::RemoveGraphFromRot(DWORD pdwRegister)
{
  IRunningObjectTable *pROT;

  if (SUCCEEDED(GetRunningObjectTable(0, &pROT)))
  {
    pROT->Revoke(pdwRegister);
    pROT->Release();
  }
}

HRESULT CDirectShowHelper::Render(IGraphBuilder *pGraph, IBaseFilter *pFilter)
{
  IPin* pOutputPin = NULL;
  HRESULT hr = CDirectShowHelper::GetPin(pFilter, PINDIR_OUTPUT, &pOutputPin);
  if (SUCCEEDED(hr))
  {
    hr = pGraph->Render(pOutputPin);
    return hr;
  }
  else
  {
    return hr;
  }
}

HRESULT CDirectShowHelper::ConnectCaptureSourceFilters(ICaptureGraphBuilder2* pGraphBuilder, IBaseFilter *pSrc, IBaseFilter *pDest)
{
  if ((pGraphBuilder == NULL) || (pSrc == NULL) || (pDest == NULL))
  {
    return E_POINTER;
  }
#if 1
  HRESULT hr = pGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, NULL, pSrc, NULL, pDest);
#else
  HRESULT hr = pGraphBuilder->RenderStream(NULL, NULL, pSrc, NULL, pDest);
#endif
  return hr;
}

HRESULT CDirectShowHelper::GetNextFilter(IBaseFilter *pFilter, PIN_DIRECTION Dir, IBaseFilter **ppNext)
{
  if (!pFilter || !ppNext) return E_POINTER;

  IEnumPins *pEnum = 0;
  IPin *pPin = 0;
  HRESULT hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr)) return hr;
  while (S_OK == pEnum->Next(1, &pPin, 0))
  {
    // See if this pin matches the specified direction.
    PIN_DIRECTION ThisPinDir;
    hr = pPin->QueryDirection(&ThisPinDir);
    if (FAILED(hr))
    {
      // Something strange happened.
      hr = E_UNEXPECTED;
      pPin->Release();
      break;
    }
    if (ThisPinDir == Dir)
    {
      // Check if the pin is connected to another pin.
      IPin *pPinNext = 0;
      hr = pPin->ConnectedTo(&pPinNext);
      if (SUCCEEDED(hr))
      {
        // Get the filter that owns that pin.
        PIN_INFO PinInfo;
        hr = pPinNext->QueryPinInfo(&PinInfo);
        pPinNext->Release();
        pPin->Release();
        pEnum->Release();
        if (FAILED(hr) || (PinInfo.pFilter == NULL))
        {
          // Something strange happened.
          return E_UNEXPECTED;
        }
        // This is the filter we're looking for.
        *ppNext = PinInfo.pFilter; // Client must release.
        return S_OK;
      }
    }
    pPin->Release();
  }
  pEnum->Release();
  // Did not find a matching filter.
  return E_FAIL;
}

HRESULT CDirectShowHelper::GetFilterConnectedToPin(IPin *pPin, IBaseFilter **ppNext)
{
  if (!pPin || !ppNext) return E_POINTER;

  // Check if the pin is connected to another pin.
  IPin *pPinNext = 0;
  HRESULT hr = pPin->ConnectedTo(&pPinNext);
  if (SUCCEEDED(hr))
  {
    // Get the filter that owns that pin.
    PIN_INFO PinInfo;
    hr = pPinNext->QueryPinInfo(&PinInfo);
    pPinNext->Release();
    if (FAILED(hr) || (PinInfo.pFilter == NULL))
    {
      // Something strange happened.
      return E_UNEXPECTED;
    }
    // This is the filter we're looking for.
    *ppNext = PinInfo.pFilter; // Client must release.
    return S_OK;
  }
  // Did not find a matching filter.
  return E_FAIL;
}

HRESULT CDirectShowHelper::FindFirstInterface(IBaseFilter *pFilter, PIN_DIRECTION Dir, REFIID iid, void **ppUnk)
{
  if (!ppUnk) return E_POINTER;
  IBaseFilter* pNext = NULL;
  HRESULT hr = CDirectShowHelper::GetNextFilter(pFilter, Dir, &pNext);
  while (SUCCEEDED(hr))
  {
    hr = pNext->QueryInterface(iid, ppUnk);
    if (SUCCEEDED(hr))
    {
      pNext->Release();
      return S_OK;
    }

    IBaseFilter* pNextFilter = pNext;
    pNext = NULL;
    hr = CDirectShowHelper::GetNextFilter(pNextFilter, Dir, &pNext);
    // now release 'previous' next
    pNextFilter->Release();
  }
  return E_FAIL;
}

HRESULT CDirectShowHelper::FindFirstFilterWithInterface(IBaseFilter *pFilter, PIN_DIRECTION Dir, REFIID iid, IBaseFilter** pResult)
{
  IBaseFilter* pNext = NULL;
  HRESULT hr = CDirectShowHelper::GetNextFilter(pFilter, Dir, &pNext);
  void **ppUnk = NULL;
  while (SUCCEEDED(hr))
  {
    hr = pNext->QueryInterface(iid, ppUnk);
    if (SUCCEEDED(hr))
    {
      // TODO: can we release the COM interface like this?!?
      IUnknown* pUnknown = (IUnknown*)*ppUnk;
      pUnknown->Release();
      *pResult = pNext;
      return S_OK;
    }

    IBaseFilter* pNextFilter = pNext;
    pNext = NULL;
    hr = CDirectShowHelper::GetNextFilter(pNextFilter, Dir, &pNext);
    // now release 'previous' next
    pNextFilter->Release();
  }
  return E_FAIL;
}


std::string CDirectShowHelper::GetTvFormatString(long lFormat)
{
  switch (lFormat)
  {
    case AnalogVideo_None:
    {
      return "None";
    }
    case AnalogVideo_NTSC_M:
    {
      return "NTSC M";
    }
    case AnalogVideo_NTSC_M_J:
    {
      return "NTSC MJ";
    }
    case AnalogVideo_NTSC_433:
    {
      return "NTSC 433";
    }
    case AnalogVideo_PAL_B:
    {
      return "PAL B";
    }
    case AnalogVideo_PAL_D:
    {
      return "PAL D";
    }
    case AnalogVideo_PAL_H:
    {
      return "PAL H";
    }
    case AnalogVideo_PAL_I:
    {
      return "PAL I";
    }
    case AnalogVideo_PAL_M:
    {
      return "PAL M";
    }
    case AnalogVideo_PAL_N:
    {
      return "PAL N";
    }
    case AnalogVideo_PAL_60:
    {
      return "PAL 60";
    }
    case AnalogVideo_SECAM_B:
    {
      return "Secam B";
    }
    case AnalogVideo_SECAM_D:
    {
      return "Secam D";
    }
    case AnalogVideo_SECAM_G:
    {
      return "Secam G";
    }
    case AnalogVideo_SECAM_H:
    {
      return "Secam H";
    }
    case AnalogVideo_SECAM_K:
    {
      return "Secam K";
    }
    case AnalogVideo_SECAM_K1:
    {
      return "Secam K1";
    }
    case AnalogVideo_SECAM_L:
    {
      return "Secam L";
    }
    case AnalogVideo_SECAM_L1:
    {
      return "Secam L1";
    }
    case AnalogVideo_PAL_N_COMBO:
    {
      return "PAL N COMBO";
    }
    default:
    {
      return "Unknown format";
    }
  }
}

HRESULT CDirectShowHelper::GetCrossBarInfo(const std::string& sVideoCaptureDevice, std::vector<std::string>& vInputPins, std::vector<std::string>& vOutputPins)
{
  // create Graph
  IGraphBuilderPtr pGraph;
  HRESULT hr = pGraph.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
  {
    return hr;
  }

  // Create the capture graph builder
  ICaptureGraphBuilder2Ptr pCaptureGraphBuilder;
  hr = pCaptureGraphBuilder.CreateInstance(CLSID_CaptureGraphBuilder2);
  if (FAILED(hr))
  {
    return hr;
  }
  pCaptureGraphBuilder->SetFiltergraph(pGraph);

  IBaseFilterPtr pSourceFilter = NULL;
  const GUID* pClsID = NULL;
  hr = CDirectShowHelper::AddFilterByCategoryAndName(pGraph, CLSID_VideoInputDeviceCategory, sVideoCaptureDevice, &pSourceFilter);
  if (SUCCEEDED(hr))
  {
    // For crossbar stuff:
    // Search for upstream crossbar
    IAMCrossbarPtr pXBar1 = NULL;
    hr = pCaptureGraphBuilder->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pSourceFilter, IID_IAMCrossbar, (void**)&pXBar1);
    if (SUCCEEDED(hr))
    {
      // Log crossbar info to screen
      std::string sResult;
      return CCrossbar::RetrieveCrossbarInfo(pXBar1, sResult, vInputPins, vOutputPins);
    }
    else
    {
      return hr;
    }
  }
  else
  {
    return hr;
  }
}

HRESULT CDirectShowHelper::VerifyVideoDevice(const std::string& sVideoSource, const std::string& sVideoCrossbarSource)
{
  std::vector<std::string> vVideoDeviceList;
  GetVideoInputs(vVideoDeviceList);
  auto findVideoDevice = std::find(vVideoDeviceList.begin(), vVideoDeviceList.end(), sVideoSource);
  if (findVideoDevice != vVideoDeviceList.end())
  {
    // found video device, check video crossbar device
    if (!sVideoCrossbarSource.empty())
    {
      std::vector<std::string> vInputPins;
      std::vector<std::string> vOutputPins;
      HRESULT hr = GetCrossBarInfo(sVideoSource, vInputPins, vOutputPins);
      if (FAILED(hr))
      {
        return hr;
      }
      else
      {
        auto findVideoCrossbarDevice = std::find(vInputPins.begin(), vInputPins.end(), sVideoCrossbarSource);
        if (findVideoCrossbarDevice != vInputPins.end())
        {
          return S_OK;
        }
        // Right video source, but crossbar device does not exist
        else
        {
          return E_FAIL;
        }
      }
    }
    else
    {
      // No video crossbar device to check
      return S_OK;
    }
  }
  else
  {
    return E_FAIL;
  }
}

HRESULT CDirectShowHelper::VerifyAudioDevice(const std::string& sAudioSource)
{
  std::vector<std::string> vAudioDeviceList;
  GetAudioInputs(vAudioDeviceList);
  auto findVideoDevice = std::find(vAudioDeviceList.begin(), vAudioDeviceList.end(), sAudioSource);
  if (findVideoDevice != vAudioDeviceList.end())
  {
    return S_OK;
  }
  else
  {
    return E_FAIL;
  }
}
HRESULT CDirectShowHelper::GetVideoDeviceCapabilityList(const std::string& sVideoSource, std::vector<std::string>& videoColorSpace,
  std::vector<double>& videoFrameRate, std::vector<std::string>& videoResolution)
{
  //Setup Camera Info
  IBaseFilterPtr videoFilter;
  HRESULT hr = CDirectShowHelper::GetFilterByCategory(&videoFilter,
    sVideoSource,
    CLSID_VideoInputDeviceCategory);
  if (SUCCEEDED(hr))
  {
    IPinPtr pOutputPin;
    hr = CDirectShowHelper::GetPin(videoFilter, PINDIR_OUTPUT, &pOutputPin);
    if (SUCCEEDED(hr))
    {
      IAMStreamConfigPtr pIamStreamConfigInterface = pOutputPin;
      // First set compression filter media type
      int nCount, nSize;
      BYTE *pSCC = NULL;
      AM_MEDIA_TYPE *pmt;

      hr = pIamStreamConfigInterface->GetNumberOfCapabilities(&nCount, &nSize);
      if (SUCCEEDED(hr))
      {
        pSCC = new BYTE[nSize];

        // Iterate over all formats to extract the available formats
        for (int i = 0; i < nCount; i++)
        {
          hr = pIamStreamConfigInterface->GetStreamCaps(i, &pmt, pSCC);
          if (SUCCEEDED(hr))
          {
            if (pmt->formattype == FORMAT_VideoInfo)
            {
              char* szType = NULL;
              std::string szSubtype = "";
              MapClassIDToString(&pmt->subtype, szSubtype);
              VIDEOINFOHEADER* pVih = (VIDEOINFOHEADER*)pmt->pbFormat;
              REFERENCE_TIME duration = pVih->AvgTimePerFrame;
              double dFrameRate = UNITS / (double)duration;
              int nWidth = pVih->bmiHeader.biWidth;
              int nHeight = pVih->bmiHeader.biHeight;
              std::string sResolution = std::to_string(nWidth) + "x" + std::to_string(nHeight);
              // items in sets are unique, duplicated items will not be stored
              videoColorSpace.push_back(szSubtype);
              videoFrameRate.push_back(dFrameRate);
              videoResolution.push_back(sResolution);
            }
          }
        }
      }
      // Now free memory
      if (pmt)
      {
        DeleteMediaType(pmt);
      }
      if (pSCC)
      {
        delete[] pSCC;
      }
    }
  }
  return hr;
}
bool CDirectShowHelper::MapClassIDToString(const GUID* pGuid, std::string& sClassIdToString)
{
  // Subtypes
  if (*pGuid == MEDIASUBTYPE_RGB24)
  {
    sClassIdToString = "MEDIASUBTYPE_RGB24";
  }
  else if (*pGuid == MEDIASUBTYPE_RGB32)
  {
    sClassIdToString = "MEDIASUBTYPE_RGB32";
  }
  else if (*pGuid == MEDIASUBTYPE_YUYV)
  {
    sClassIdToString = "MEDIASUBTYPE_YUYV";
  }
  else if (*pGuid == MEDIASUBTYPE_IYUV)
  {
    sClassIdToString = "MEDIASUBTYPE_IYUV";
  }
  else if (*pGuid == MEDIASUBTYPE_YUY2)
  {
    sClassIdToString = "MEDIASUBTYPE_YUY2";
  }
  else if (*pGuid == MEDIASUBTYPE_YVYU)
  {
    sClassIdToString = "MEDIASUBTYPE_YVYU";
  }
  else if (*pGuid == MEDIASUBTYPE_UYVY)
  {
    sClassIdToString = "MEDIASUBTYPE_UYVY";
  }
  else if (*pGuid == MEDIASUBTYPE_RGB565)
  {
    sClassIdToString = "MEDIASUBTYPE_RGB565";
  }
  else if (*pGuid == MEDIASUBTYPE_RGB555)
  {
    sClassIdToString = "MEDIASUBTYPE_RGB555";
  }
  else
  {
    sClassIdToString = "Unknown";
    return false;
  }
  return true;
}

bool CDirectShowHelper::CanSupplyType(IPin* pPin, const GUID* pMajorType)
{
  IEnumMediaTypesPtr pEnum;
  pPin->EnumMediaTypes(&pEnum);
  AM_MEDIA_TYPE* pmt;
  while (pEnum->Next(1, &pmt, NULL) == S_OK)
  {
    CMediaType mt(*pmt);
    DeleteMediaType(pmt);
    if (*mt.Type() == *pMajorType)
    {
      return true;
    }
  }
  return false;
}

HRESULT CDirectShowHelper::GetFileFramerate(const std::string& sFile, double& dFramerate, std::string& error)
{
  dFramerate = -1.0;
  // create Graph
  IGraphBuilderPtr pGraph;
  HRESULT hr = pGraph.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
  {
    error = "Failed to create filter graph";
    return hr;
  }

  // manual construction of graph is required for machines with no audio device
  // avi demux
  auto pos = sFile.rfind(".");
  if (pos == std::string::npos)
    return E_FAIL;
  std::string sExt = sFile.substr(pos);
  const wchar_t* wszDemux = NULL;
  const std::wstring MP4Demux(L"{025BE2E4-1787-4DA4-A585-C5B2B9EEB57C}");
  const std::wstring AviDemux(L"{1B544C20-FD0B-11CE-8C63-00AA0044B51E}");
  if (sExt == ".mp4" || sExt == ".m4v")
  {
    wszDemux = MP4Demux.c_str();
  }
  else if (sExt == ".avi")
  {
    wszDemux = AviDemux.c_str();
  }
  else
  {
    // unknown file type
    error = "Unsupported file type: " + sExt;
    return E_FAIL;
  }
  // file source
  std::wstring wsName = stringToWideString(sFile);
  IBaseFilterPtr pFile;
  hr = pGraph->AddSourceFilter(wsName.c_str(), wsName.c_str(), &pFile);
  if (FAILED(hr))
  {
    error = "Failed to add source filter for " + sFile;
    return hr;
  }

  // demux
  GUID clsid;
  hr = CLSIDFromString(wszDemux, &clsid);
  if (FAILED(hr))
  {
    error = "Failed to convert string to demux class id.";
    HRESULT hrTemp = pGraph->RemoveFilter(pFile);
    return hr;
  }

  // start with source filter for first pin
  // -- expect the source to expose a muxed type
  IPinPtr pOut;
  hr = CDirectShowHelper::GetUnconnectedPin(pFile, PINDIR_OUTPUT, &pOut, &MEDIATYPE_Stream);
  if (FAILED(hr))
  {
    error = "Failed to get unconnected pin.";
    HRESULT hrTemp = pGraph->RemoveFilter(pFile);
    return hr;
  }
  IBaseFilterPtr pDemux;
  hr = AddFilterByCLSID(pGraph, clsid, L"Demux", &pDemux);
  if (FAILED(hr))
  {
    error = "Failed to add demux.";
    HRESULT hrTemp = pGraph->RemoveFilter(pFile);
    return hr;
  }
  hr = ConnectFilters(pGraph, pOut, pDemux);
  if (FAILED(hr))
  {
    error = "Failed to connect demux.";
    HRESULT hrTemp = pGraph->RemoveFilter(pFile);
    hrTemp = pGraph->RemoveFilter(pDemux);
    return hr;
  }
  // now iterate over output pins of demux
  // recurse down all connected pins
  IEnumPinsPtr pEnum;
  pDemux->EnumPins(&pEnum);
  IPinPtr pPin;
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    // is this output and connected?
    PIN_DIRECTION dirThis;
    pPin->QueryDirection(&dirThis);

    IPinPtr pPeer;
    if ((dirThis == PINDIR_OUTPUT) /*&& (pPin->ConnectedTo(&pPeer) == S_OK)*/)
    {
      IEnumMediaTypesPtr pEnum;
      pPin->EnumMediaTypes(&pEnum);
      AM_MEDIA_TYPE* pmt;
      while (pEnum->Next(1, &pmt, NULL) == S_OK)
      {
        CMediaType mt(*pmt);
        DeleteMediaType(pmt);
        if (mt.formattype == FORMAT_VideoInfo)
        {
          VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)mt.pbFormat;
          dFramerate = 10000000.0 / pVih->AvgTimePerFrame;
          HRESULT hrTemp = pGraph->RemoveFilter(pFile);
          hrTemp = pGraph->RemoveFilter(pDemux);
          return S_OK;
        }
        else if (mt.formattype == FORMAT_VideoInfo2)
        {
          VIDEOINFOHEADER2 *pVih = (VIDEOINFOHEADER2*)mt.pbFormat;
          dFramerate = 10000000.0 / pVih->AvgTimePerFrame;
          HRESULT hrTemp = pGraph->RemoveFilter(pFile);
          hrTemp = pGraph->RemoveFilter(pDemux);
          return S_OK;
        }
        else if (mt.formattype == FORMAT_MPEG2Video)
        {
          MPEG2VIDEOINFO* pMh1 = (MPEG2VIDEOINFO*)mt.Format();
          VIDEOINFOHEADER2 *pVih = (VIDEOINFOHEADER2*)&(pMh1->hdr);
          dFramerate = 10000000.0 / pVih->AvgTimePerFrame;
          HRESULT hrTemp = pGraph->RemoveFilter(pFile);
          hrTemp = pGraph->RemoveFilter(pDemux);
          return S_OK;
        }
      }
    }
  }

  HRESULT hrTemp = pGraph->RemoveFilter(pFile);
  hrTemp = pGraph->RemoveFilter(pDemux);
  return E_FAIL;
}

HRESULT CDirectShowHelper::GetFileVideoInfo(const std::string& sFile, int& iHeight, int& iWidth, GUID& MediaSubtype)
{
  iHeight = -1;
  iWidth = -1;
  // create Graph
  IGraphBuilderPtr pGraph;
  HRESULT hr = pGraph.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
  {
    return hr;
  }
  // manual construction of graph is required for machines with no audio device
  // avi demux
  auto pos = sFile.rfind(".");
  if (pos == std::string::npos)
    return E_FAIL;
  std::string sExt = sFile.substr(pos);
  const wchar_t* wszDemux = NULL;
  const std::wstring MP4Demux(L"{025BE2E4-1787-4DA4-A585-C5B2B9EEB57C}");
  const std::wstring AviDemux(L"{1B544C20-FD0B-11CE-8C63-00AA0044B51E}");
  if (sExt == ".mp4" || sExt == ".m4v")
  {
    wszDemux = MP4Demux.c_str();
  }
  else if (sExt == ".avi")
  {
    wszDemux = AviDemux.c_str();
  }
  else
  {
    // unknown file type
    return E_FAIL;
  }
  // file source
  std::wstring wsName = stringToWideString(sFile);
  IBaseFilterPtr pFile;
  hr = pGraph->AddSourceFilter(wsName.c_str(), wsName.c_str(), &pFile);
  if (FAILED(hr))
  {
    return hr;
  }

  // demux
  GUID clsid;
  hr = CLSIDFromString(wszDemux, &clsid);
  if (FAILED(hr))
  {
    return hr;
  }

  // start with source filter for first pin
  // -- expect the source to expose a muxed type
  IPinPtr pOut;
  hr = CDirectShowHelper::GetUnconnectedPin(pFile, PINDIR_OUTPUT, &pOut, &MEDIATYPE_Stream);
  if (FAILED(hr))
  {
    return hr;
  }
  IBaseFilterPtr pDemux;
  hr = AddFilterByCLSID(pGraph, clsid, L"Demux", &pDemux);
  if (FAILED(hr))
  {
    return hr;
  }
  hr = ConnectFilters(pGraph, pOut, pDemux);
  if (FAILED(hr))
  {
    return hr;
  }
  // now iterate over output pins of demux
  // recurse down all connected pins
  IEnumPinsPtr pEnum;
  pDemux->EnumPins(&pEnum);
  IPinPtr pPin;
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    // is this output and connected?
    PIN_DIRECTION dirThis;
    pPin->QueryDirection(&dirThis);

    IPinPtr pPeer;
    if ((dirThis == PINDIR_OUTPUT) /*&& (pPin->ConnectedTo(&pPeer) == S_OK)*/)
    {
      IEnumMediaTypesPtr pEnum;
      pPin->EnumMediaTypes(&pEnum);
      AM_MEDIA_TYPE* pmt;
      while (pEnum->Next(1, &pmt, NULL) == S_OK)
      {
        CMediaType mt(*pmt);
        // check if the width and height is correct
        // check if the media subtype for mp4 and m4v is correct
        if (FAILED(hr))
        {
          return hr;
        }
        DeleteMediaType(pmt);
        MediaSubtype = mt.subtype;
        if (mt.formattype == FORMAT_VideoInfo)
        {
          VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)mt.pbFormat;
          const BITMAPINFOHEADER *pBmh = &pVih->bmiHeader;
          iHeight = pBmh->biHeight;
          iWidth = pBmh->biWidth;
          return S_OK;
        }
        else if (mt.formattype == FORMAT_VideoInfo2)
        {
          VIDEOINFOHEADER2 *pVih2 = (VIDEOINFOHEADER2*)mt.pbFormat;
          const BITMAPINFOHEADER *pBmh = &pVih2->bmiHeader;
          iHeight = pBmh->biHeight;
          iWidth = pBmh->biWidth;
          return S_OK;
        }
        else if (mt.formattype == FORMAT_MPEG2Video)
        {
          MPEG2VIDEOINFO* pMh1 = (MPEG2VIDEOINFO*)mt.Format();
          const BITMAPINFOHEADER *pBmh = &pMh1->hdr.bmiHeader;
          iHeight = pBmh->biHeight;
          iWidth = pBmh->biWidth;
          return S_OK;
        }
      }
    }
  }
  return E_FAIL;
}

HRESULT CDirectShowHelper::FindStreamSource(IBaseFilter* pFilter, const GUID* pMajorType, IPin** ppPin)
{
  // is it on this filter?
  HRESULT hr = CDirectShowHelper::GetUnconnectedPin(pFilter, PINDIR_OUTPUT, ppPin, pMajorType);
  if (hr == S_OK)
  {
    return hr;
  }

  // recurse down all connected pins
  IEnumPinsPtr pEnum;
  pFilter->EnumPins(&pEnum);
  IPinPtr pPin;
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    // is this output and connected?
    PIN_DIRECTION dirThis;
    pPin->QueryDirection(&dirThis);

    IPinPtr pPeer;
    if ((dirThis == PINDIR_OUTPUT) && (pPin->ConnectedTo(&pPeer) == S_OK))
    {
      // traverse to downstream filter
      PIN_INFO pi;
      pPeer->QueryPinInfo(&pi);
      IBaseFilterPtr pfPeer(pi.pFilter, 0);

      // recurse to find source on this filter
      hr = FindStreamSource(pfPeer, pMajorType, ppPin);
      if (hr != S_FALSE)
      {
        // fail if error, or success - pin found
        return hr;
      }
    }
  }
  // not on this branch
  return S_FALSE;
}

HRESULT CDirectShowHelper::FileHasVideoAndAudio(const std::string& sFile, bool& bHasVideo, bool& bHasAudio)
{
  bHasVideo = false; bHasAudio = false;
  // create Graph
  IGraphBuilderPtr pGraph;
  HRESULT hr = pGraph.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
  {
    return hr;
  }

  // manual construction of graph is required for machines with no audio device
  auto pos = sFile.rfind(".");
  if (pos == std::string::npos)
    return E_FAIL;
  std::string sExt = sFile.substr(pos);
  const wchar_t* wszDemux = NULL;
  const std::wstring MP4Demux(L"{025BE2E4-1787-4DA4-A585-C5B2B9EEB57C}");
  const std::wstring AviDemux(L"{1B544C20-FD0B-11CE-8C63-00AA0044B51E}");
  if (sExt == ".mp4" || sExt == ".m4v")
  {
    wszDemux = MP4Demux.c_str();
  }
  else if (sExt == ".avi")
  {
    wszDemux = AviDemux.c_str();
  }
  else
  {
    // unknown file type
    return E_FAIL;
  }
  // file source
  std::wstring wsName = stringToWideString(sFile);
  IBaseFilterPtr pFile;
  hr = pGraph->AddSourceFilter(wsName.c_str(), wsName.c_str(), &pFile);
  if (FAILED(hr))
  {
    return hr;
  }

  // demux
  GUID clsid;
  hr = CLSIDFromString(wszDemux, &clsid);
  if (FAILED(hr))
  {
    return hr;
  }

  // start with source filter for first pin
  // -- expect the source to expose a muxed type
  IPinPtr pOut;
  hr = CDirectShowHelper::GetUnconnectedPin(pFile, PINDIR_OUTPUT, &pOut, &MEDIATYPE_Stream);
  if (FAILED(hr))
  {
    return hr;
  }
  IBaseFilterPtr pDemux;
  hr = AddFilterByCLSID(pGraph, clsid, L"Demux", &pDemux);
  if (FAILED(hr))
  {
    return hr;
  }
  hr = ConnectFilters(pGraph, pOut, pDemux);
  if (FAILED(hr))
  {
    return hr;
  }
  // now iterate over output pins of demux
  // recurse down all connected pins
  IEnumPinsPtr pEnum;
  pDemux->EnumPins(&pEnum);
  IPinPtr pPin;
  while (pEnum->Next(1, &pPin, NULL) == S_OK)
  {
    // is this output and connected?
    PIN_DIRECTION dirThis;
    pPin->QueryDirection(&dirThis);

    IPinPtr pPeer;
    if ((dirThis == PINDIR_OUTPUT) /*&& (pPin->ConnectedTo(&pPeer) == S_OK)*/)
    {
      if (CanSupplyType(pPin, &MEDIATYPE_Audio))
      {
        bHasAudio = true;
      }
      else if (CanSupplyType(pPin, &MEDIATYPE_Video))
      {
        bHasVideo = true;
      }
      if (bHasAudio && bHasVideo) break;
    }
  }
  return S_OK;
}
