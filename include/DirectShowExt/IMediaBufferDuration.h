/** @file

MODULE:
 
TAG: 

FILE NAME: 

DESCRIPTION: 

COPYRIGHT: (c)CSIR 2007-2018 all rights reserved

LICENSE: Software License Agreement (BSD License)

RESTRICTIONS: 
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this 
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may 
be used to endorse or promote products derived from this software without specific 
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================

*/

#pragma once

/// This interface allows us to obtain 'simple' buffer information such as the desired
/// minimum duration in the buffer and the actual media duration in the buffer
/// This interface is used by the NetworkReferenceClock class to perform rate matching 
/// in DirectShow source filters

class IMediaBufferDuration
{
public:

  IMediaBufferDuration()
    :m_dMinDuration(1.0) // Defaulting to one second
  {}

  IMediaBufferDuration(double dMinDuration)
    :m_dMinDuration(dMinDuration)
  {}

  virtual ~IMediaBufferDuration(){}

  /// Desired 'minimum' duration of media in buffer
  double getMinDurationSeconds() const { return m_dMinDuration; }
  void setMinDurationSeconds(double val) { m_dMinDuration = val; }

  /// Actual duration of media in buffer
  virtual double getDurationSeconds() const = 0;

protected:

  /// the desired 'minimum' duration in seconds to be buffered
  double m_dMinDuration;
};



