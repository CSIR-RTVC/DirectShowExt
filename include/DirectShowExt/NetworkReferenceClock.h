/** @file

MODULE				: vpp

FILE NAME			: NetworkReferenceClock.h

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
#pragma once

#pragma warning(push)     // disable for this header only
#pragma warning(disable:4312) 
// DirectShow
#include <Streams.h>
#pragma warning(pop)      // restore original warning level

class IMediaBufferDuration;

class NetworkReferenceClock : public CBaseReferenceClock
{
public:

  NetworkReferenceClock(__in_opt LPCTSTR pName, 
                        __inout_opt LPUNKNOWN pUnk, 
                        __inout HRESULT *phr, 
                        __inout_opt CAMSchedule * pSched = 0 );
  
  virtual ~NetworkReferenceClock();

  /// Mutator for packet manager
  void setMediaBufferDuration(IMediaBufferDuration* pMediaBuffer) { m_pMediaBuffer =  pMediaBuffer; }

  /// method to reset rate matching state
  void reset();

  /// CBaseReferenceClock
  virtual REFERENCE_TIME GetPrivateTime();

private:

  /// Has first sample been received
  bool m_bFirst;
  /// current rate adapter
  double m_dCurrentRateAdapter;

  /// Stores the time that was previously returned via the GetPrivateTime call
  /// Used to calculate next time
  REFERENCE_TIME m_tPreviousTime;
  /// Stores the system time reference
  /// Used to calculate the elapsed time since the last call
  REFERENCE_TIME m_tPreviousRefTime;

  /// IMediaBuffer interface used to obtain information about media buffer
  IMediaBufferDuration* m_pMediaBuffer;
  
  /// Interval that checks take place in
  REFERENCE_TIME m_tCheckInterval;
  /// Stores the time of the next rate adaptation check
  REFERENCE_TIME m_tNextCheck;
  /// Currently not used, stores the duration of the buffer during the previous call to GetPrivateTime
  /// This could be used to see whether the buffer is growing or shrinking
  double m_dPreviousDuration;
};
