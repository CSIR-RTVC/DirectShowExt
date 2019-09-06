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
//This file needs to be included - else we get a linker error for the GUID
#include <INITGUID.H>

// {15377191-6942-4E6E-8D5C-A958D39FFD95}
DEFINE_GUID(IID_IFilterInfoSourceInterface,
  0x15377191, 0x6942, 0x4e6e, 0x8d, 0x5c, 0xa9, 0x58, 0xd3, 0x9f, 0xfd, 0x95);

#undef  INTERFACE
#define INTERFACE IFilterInfoSourceInterface
DECLARE_INTERFACE_(IFilterInfoSourceInterface, IUnknown)
{
  /// @brief Method to retrieve last set value
  /// @param value[out] Buffer for value to be stored in
  /// @param[in/out] buffersize Size of output buffer. Also stores length of string in buffer after output
  STDMETHOD(GetMeasurement)(char* value, int* buffersize) = 0;
};
