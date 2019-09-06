#pragma once
//------------------------------------------------------------------------------
// File: Crossbar.h
//
// Desc: DirectShow sample code - definition of class for controlling
//       video crossbars.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include <string>
#include <sstream>

//------------------------------------------------------------------------------
// Name: class CClass
// Desc: This class contains routing information for the capture data
//------------------------------------------------------------------------------
class CRouting {
public:
  class CRouting       *pLeftRouting;
  class CRouting       *pRightRouting;
  LONG                 VideoInputIndex;
  LONG                 VideoOutputIndex;
  LONG                 AudioInputIndex;
  LONG                 AudioOutputIndex;
  IAMCrossbar         *pXbar;
  LONG                 InputPhysicalType;
  LONG                 OutputPhysicalType;
  LONG                 Depth;

  CRouting() {};
  ~CRouting() {};
};

typedef CGenericList<CRouting> CRoutingList;

//------------------------------------------------------------------------------
// Name: class CCrossbar
// Desc: The actual helper class for Crossbars
//------------------------------------------------------------------------------
class CCrossbar
{
private:
  IPin                    *m_pStartingPin;
  CRouting                 m_RoutingRoot;
  CRoutingList            *m_RoutingList;
  int                      m_CurrentRoutingIndex;

  HRESULT BuildRoutingList(
    IPin     *pStartingInputPin,
    CRouting *pCRouting,
    int       Depth);
  HRESULT SaveRouting(CRouting *pRoutingNew);
  HRESULT DestroyRoutingList();
  BOOL    StringFromPinType(TCHAR *pc, int nSize, long lType);

  HRESULT GetCrossbarIPinAtIndex(
    IAMCrossbar *pXbar,
    LONG PinIndex,
    BOOL IsInputPin,
    IPin ** ppPin);
  HRESULT GetCrossbarIndexFromIPin(
    IAMCrossbar * pXbar,
    LONG * PinIndex,
    BOOL IsInputPin,
    IPin * pPin);

public:

  CCrossbar(IPin *pPin, HRESULT *phr);
  ~CCrossbar();

  HRESULT GetInputCount(LONG *pCount);
  HRESULT GetInputType(LONG Index, LONG * PhysicalType);
  HRESULT GetInputName(LONG Index, TCHAR * pName, LONG NameSize);
  HRESULT SetInputIndex(LONG Index);
  HRESULT GetInputIndex(LONG *Index);

  // Helper function to get the index of a crossbar pin with name
  static bool GetCrossBarPinIndices(IAMCrossbar *pXBar, const std::string& sPhysicalPinName, long& nPinIndex, long& nRelatedPinIndex)
  {
    HRESULT hr;
    long cOutput = -1, cInput = -1;
    hr = pXBar->get_PinCounts(&cOutput, &cInput);

    for (int i = 0; i < cInput; i++)
    {
      long lRelated = -1, lType = -1;

      hr = pXBar->get_CrossbarPinInfo(TRUE, i, &lRelated, &lType);

      if (hr == S_OK)
      {
        std::string sName = std::string(GetPhysicalPinName(lType));
        if (sName == sPhysicalPinName)
        {
          //We found the pinn
          nPinIndex = i;
          nRelatedPinIndex = lRelated;
          return true;
        }
      }
    }
    // We didn't find it
    nPinIndex = -1;
    nRelatedPinIndex = -1;
    return false;
  }

  // Helper function to associate a name with the type.
  static const char * GetPhysicalPinName(long lType)
  {
    switch (lType)
    {
      case PhysConn_Video_Tuner:            return "Video Tuner";
      case PhysConn_Video_Composite:        return "Video Composite";
      case PhysConn_Video_SVideo:           return "S-Video";
      case PhysConn_Video_RGB:              return "Video RGB";
      case PhysConn_Video_YRYBY:            return "Video YRYBY";
      case PhysConn_Video_SerialDigital:    return "Video Serial Digital";
      case PhysConn_Video_ParallelDigital:  return "Video Parallel Digital";
      case PhysConn_Video_SCSI:             return "Video SCSI";
      case PhysConn_Video_AUX:              return "Video AUX";
      case PhysConn_Video_1394:             return "Video 1394";
      case PhysConn_Video_USB:              return "Video USB";
      case PhysConn_Video_VideoDecoder:     return "Video Decoder";
      case PhysConn_Video_VideoEncoder:     return "Video Encoder";

      case PhysConn_Audio_Tuner:            return "Audio Tuner";
      case PhysConn_Audio_Line:             return "Audio Line";
      case PhysConn_Audio_Mic:              return "Audio Microphone";
      case PhysConn_Audio_AESDigital:       return "Audio AES/EBU Digital";
      case PhysConn_Audio_SPDIFDigital:     return "Audio S/PDIF";
      case PhysConn_Audio_SCSI:             return "Audio SCSI";
      case PhysConn_Audio_AUX:              return "Audio AUX";
      case PhysConn_Audio_1394:             return "Audio 1394";
      case PhysConn_Audio_USB:              return "Audio USB";
      case PhysConn_Audio_AudioDecoder:     return "Audio Decoder";

      default:                              return "Unknown Type";
    }
  }


  static HRESULT RetrieveCrossbarInfo(IAMCrossbar *pXBar, std::string& sResult, std::vector<std::string>& vInputPins, std::vector<std::string>& vOutputPins)
  {
    HRESULT hr;
    long cOutput = -1, cInput = -1;
    hr = pXBar->get_PinCounts(&cOutput, &cInput);
    if (FAILED(hr)) return hr;

    std::ostringstream ostr;

    ostr << "-------- Output pins ------------" << std::endl;
    for (long i = 0; i < cOutput; i++)
    {
      long lRelated = -1, lType = -1, lRouted = -1, lInputIndex = -1;

      hr = pXBar->get_CrossbarPinInfo(FALSE, i, &lRelated, &lType);
      if (FAILED(hr)) { return hr; }

      hr = pXBar->get_IsRoutedTo(i, &lRouted);
      if (FAILED(hr)) { return hr; }

      ostr << "Output pin " << i << ": " << GetPhysicalPinName(lType) << std::endl;
      vOutputPins.push_back(GetPhysicalPinName(lType));
      if (lRelated == -1)
      {
        // No pin is related to this
        ostr << "No related pins" << std::endl;
      }
      else
      {
        // We have the index of the related input pin
        ostr << "Related pin: " << CCrossbar::GetPhysicalPinName(lRelated) << std::endl;
      }

      //printf("\tRelated out: %d, Routed in: %d\n", lRelated, lRouted);
      //printf("\tSwitching Matrix: ");

      if (hr == S_FALSE)
      {
        // No input pin is routed to this output pin
        ostr << "No connected input pin" << std::endl;
      }
      else if (hr == S_OK)
      {
        // We've got the pin index of the input pin
        ostr << "Connected input pin: " << lRouted << std::endl;
      }
      else
      {
        // NULL POINTER ARG
      }

      ostr << "Switching matrix:" << std::endl;
      for (long j = 0; j < cInput; j++)
      {
        hr = pXBar->CanRoute(i, j);
        ostr << i << " to " << j << (S_OK == hr ? " Yes" : " No") << std::endl;
      }
      ostr << std::endl;
    }

    ostr << "-------- Input pins ------------" << std::endl;
    for (int i = 0; i < cInput; i++)
    {
      long lRelated = -1, lType = -1;

      hr = pXBar->get_CrossbarPinInfo(TRUE, i, &lRelated, &lType);
      if (FAILED(hr)) { return hr; }

      ostr << "Input pin " << i << ":" << GetPhysicalPinName(lType) << std::endl;
      vInputPins.push_back(GetPhysicalPinName(lType));

      long lRelated2 = -1, lType2 = -1;
      hr = pXBar->get_CrossbarPinInfo(TRUE, lRelated, &lRelated2, &lType2);
      if (FAILED(hr)) { return hr; }

      ostr << "Related pin " << lRelated << ": " << GetPhysicalPinName(lType2) << std::endl;
    }
    sResult = ostr.str();
    return S_OK;
  }
};
