/** @file

MODULE				: vpp

FILE NAME			: ComTypeDefs.h

DESCRIPTION			: Stores COM typedefs

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

#include <comdef.h>

#include "SettingsInterface.h"
#include "StatusInterface.h"
#include "CodecControlInterface.h"
#include "IFileInfoSourceInterface.h"

// DirectShow COM smart pointers
_COM_SMARTPTR_TYPEDEF(IGraphBuilder, IID_IGraphBuilder);
_COM_SMARTPTR_TYPEDEF(IMediaControl, IID_IMediaControl);
_COM_SMARTPTR_TYPEDEF(IMediaEvent, IID_IMediaEvent);
_COM_SMARTPTR_TYPEDEF(IMediaEventEx, IID_IMediaEventEx);
_COM_SMARTPTR_TYPEDEF(IMediaEventSink, IID_IMediaEventSink);
_COM_SMARTPTR_TYPEDEF(IMediaSeeking, IID_IMediaSeeking);
_COM_SMARTPTR_TYPEDEF(IBaseFilter, IID_IBaseFilter);
_COM_SMARTPTR_TYPEDEF(ICaptureGraphBuilder2, IID_ICaptureGraphBuilder2);
_COM_SMARTPTR_TYPEDEF(ICreateDevEnum, IID_ICreateDevEnum);
_COM_SMARTPTR_TYPEDEF(IPin, IID_IPin);
_COM_SMARTPTR_TYPEDEF(IFileSourceFilter, IID_IFileSourceFilter);
_COM_SMARTPTR_TYPEDEF(IFileSinkFilter, IID_IFileSinkFilter);
_COM_SMARTPTR_TYPEDEF(IAMAnalogVideoDecoder, IID_IAMAnalogVideoDecoder);
_COM_SMARTPTR_TYPEDEF(IEnumMediaTypes, IID_IEnumMediaTypes);
_COM_SMARTPTR_TYPEDEF(IEnumPins, IID_IEnumPins);

//Sample Grabber Interface
#include <QEdit.h>
_COM_SMARTPTR_TYPEDEF(ISampleGrabber, IID_ISampleGrabber);
_COM_SMARTPTR_TYPEDEF(ISampleGrabberCB, IID_ISampleGrabberCB);
// For compression
_COM_SMARTPTR_TYPEDEF(IAMStreamConfig, IID_IAMStreamConfig);
// Audio buffer latency configuration
_COM_SMARTPTR_TYPEDEF(IAMBufferNegotiation, IID_IAMBufferNegotiation);
// IReferenceClock
_COM_SMARTPTR_TYPEDEF(IReferenceClock, IID_IReferenceClock);
_COM_SMARTPTR_TYPEDEF(IMediaFilter, IID_IMediaFilter);
_COM_SMARTPTR_TYPEDEF(IAMCrossbar, IID_IAMCrossbar);

//Smart pointers for VPP COM interfaces
_COM_SMARTPTR_TYPEDEF(ISettingsInterface, IID_ISettingsInterface);
_COM_SMARTPTR_TYPEDEF(IStatusInterface, IID_IStatusInterface);
_COM_SMARTPTR_TYPEDEF(ICodecControlInterface, IID_ICodecControlInterface);
_COM_SMARTPTR_TYPEDEF(IFilterInfoSourceInterface, IID_IFilterInfoSourceInterface);
