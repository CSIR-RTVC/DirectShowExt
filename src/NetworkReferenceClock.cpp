/** @file

MODULE				: vpp

FILE NAME			: NetworkReferenceClock.cpp

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
#include "NetworkReferenceClock.h"
#include <cassert>
#include <numeric>
#include "IMediaBufferDuration.h"

NetworkReferenceClock::NetworkReferenceClock(__in_opt LPCTSTR pName, __inout_opt LPUNKNOWN pUnk, __inout HRESULT *phr, __inout_opt CAMSchedule * pSched )
  :CBaseReferenceClock(pName, pUnk, phr, pSched),
  m_bFirst(false),
  m_dCurrentRateAdapter(1.0),
  m_tPreviousTime(0),
  m_tPreviousRefTime(0),
  m_pMediaBuffer(NULL),
  m_tCheckInterval(20000000),  // check every two seconds
  m_tNextCheck(0),
  m_dPreviousDuration(0.0)
{

}

NetworkReferenceClock::~NetworkReferenceClock()
{

}

void NetworkReferenceClock::reset()
{
  m_bFirst = false;
  m_dCurrentRateAdapter = 1.0;

  m_tPreviousTime = 0;
  m_tPreviousRefTime = 0;

  m_tNextCheck = 0;
  m_dPreviousDuration = 0.0;
}


REFERENCE_TIME NetworkReferenceClock::GetPrivateTime()
{
  // media buffer MUST have been configured at this stage
  assert(m_pMediaBuffer);

  // this component can be in two states when asked for the time
  // 1) it hasn't had enough samples so just return the time as when retrieved by GetPrivateTime
  // 2) rate matching has started and we need to apply the rate adapter to the interval betweent the
  //    previous time we were called and now effectively slowing down or speeding up time.
  REFERENCE_TIME tNow = CBaseReferenceClock::GetPrivateTime();

  if (!m_bFirst)
  {
    // set flag
    m_bFirst = true;
    // calc time to check
    m_tNextCheck = tNow + m_tCheckInterval;
    m_dPreviousDuration = m_pMediaBuffer->getDurationSeconds();
#if 0
	VLOG(5) << "Seed values: Min " << m_pMediaBuffer->getMinDurationSeconds() << "s Rate adapter: " << m_dCurrentRateAdapter ;
#endif
  }
  else
  {
    if (tNow >= m_tNextCheck)
    {
      //query media packet manager for queue duration
      //double dDuration = m_pMediaBuffer->getMinDurationSeconds();
      double dCurrentDuration = m_pMediaBuffer->getDurationSeconds();
      double dMinDuration  = m_pMediaBuffer->getMinDurationSeconds();
      // check if the buffer is
      // - bigger than min and growing
      if (dCurrentDuration > dMinDuration)
      {
        // calc ratio
        double dRatio = dCurrentDuration/dMinDuration;

        // Tolerate half the desired queue size
        if (dRatio > 1.3)
        {
          m_dCurrentRateAdapter = 1 * dRatio;
#if 0
          VLOG(15) << "Checking values: Min " << dMinDuration << "s Previous: " <<  m_dPreviousDuration << "s Current: " << dCurrentDuration << "s Rate adapter: " << m_dCurrentRateAdapter ;
#endif
		}

        //// use fixed values for now
        //if (dRatio > 5)
        //{
        //  m_dCurrentRateAdapter = 1.5;
		// VLOG(15) << "Checking values: Min " << dMinDuration << " Previous: " << m_dPreviousDuration << " Current: " << dCurrentDuration << " Rate adapter: " << m_dCurrentRateAdapter;
        //}
        //else if (dRatio > 2 && dRatio < 5)
        //{
        //  m_dCurrentRateAdapter = 1.3;
		// VLOG(15) << "Checking values: Min " << dMinDuration << " Previous: " << m_dPreviousDuration << " Current: " << dCurrentDuration << " Rate adapter: " << m_dCurrentRateAdapter;
		//}
        //else
        //{
        //  m_dCurrentRateAdapter = 1.1;
        //  VLOG(15) << "Checking values: Min " << dMinDuration << " Previous: " << m_dPreviousDuration << " Current: " << dCurrentDuration << " Rate adapter: " << m_dCurrentRateAdapter ;
        //}
      }
      else
      {
        // Do nothing: leave multiplier as is
      }

#if 0
      VLOG(5) << "Checking values: Min " << dMinDuration << " Previous: " << m_dPreviousDuration << " Current: " << dCurrentDuration << " Rate adapter: " << m_dCurrentRateAdapter ;
#endif
      // Update previous duration
      m_dPreviousDuration = dCurrentDuration;

      // schedule next check in interval
      m_tNextCheck = tNow + m_tCheckInterval;
    }
  }

  // Calculate time elapsed from last call: = interval elapsed
  REFERENCE_TIME tElapsed = tNow - m_tPreviousRefTime;

  // time to return is time elapsed multiplied by current rate adapter
  REFERENCE_TIME tTimeToReturn = m_tPreviousTime + static_cast<REFERENCE_TIME>(tElapsed * m_dCurrentRateAdapter);

#if 0
  VLOG(5) << "Private time: " << tNow << " Rate adapter: " << m_dCurrentRateAdapter ;
#endif

  // update reference times for next call
  m_tPreviousRefTime = tNow;
  m_tPreviousTime = tTimeToReturn;

  return tTimeToReturn;
}
