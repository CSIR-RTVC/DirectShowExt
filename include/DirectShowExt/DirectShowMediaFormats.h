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

//This file stores all RTVC DirectShow Media PipeLine Format Types
#include <INITGUID.H>
/// Custom Meraka YUV420 media format
// {C528B79F-F025-4eb7-92B0-46CE8167E9F8}
static const GUID MEDIASUBTYPE_YUV420P_S = 
{ 0xc528b79f, 0xf025, 0x4eb7, { 0x92, 0xb0, 0x46, 0xce, 0x81, 0x67, 0xe9, 0xf8 } };

/// Custom RTVC H263 media format
// {BBE21DA7-2092-4d4d-AD81-3B6998A18662}
static const GUID MEDIASUBTYPE_H263M = 
{ 0xbbe21da7, 0x2092, 0x4d4d, { 0xad, 0x81, 0x3b, 0x69, 0x98, 0xa1, 0x86, 0x62 } };

/// Custom RTVC H264 media format
// {BDF25152-046B-4509-8E55-6C73831C8DC4}
static const GUID MEDIASUBTYPE_H264M = 
{ 0xbdf25152, 0x46b, 0x4509, { 0x8e, 0x55, 0x6c, 0x73, 0x83, 0x1c, 0x8d, 0xc4 } };
/// Custom RTVC H264 media format
// {BDF25152-046B-4509-8E55-6C73831C8DC4}
static const GUID MEDIASUBTYPE_VPP_H264 =
{ 0xbdf25152, 0x46b, 0x4509, { 0x8e, 0x55, 0x6c, 0x73, 0x83, 0x1c, 0x8d, 0xc4 } };

#if 1
// from MainConcept Demo Codec SDK
// {55B845A5-8169-4BE7-BA63-6C4C2C01266D}
static const GUID MEDIASUBTYPE_MC_H265 =
{ 0x55B845A5, 0x8169, 0x4bE7,{ 0xBA, 0x63, 0x6C, 0x4C, 0x2C, 0x01, 0x26, 0x6D } };

// {31637668-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_hvc1 =
{ 0x31637668, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// {31435648-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_HVC1 =
{ 0x31435648, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// {63766568-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_hevc =
{ 0x63766568, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// {43564548-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_HEVC =
{ 0x43564548, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// {35363268-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_h265 =
{ 0x35363268, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// {35363248-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_H265 =
{ 0x35363248, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
#else
// TO BE USED
// {C8C6A6D7-893C-4EEA-A562-0FB5915F47CD}
static const GUID MEDIASUBTYPE_H265 =
{ 0xc8c6a6d7, 0x893c, 0x4eea, { 0xa5, 0x62, 0xf, 0xb5, 0x91, 0x5f, 0x47, 0xcd } };
// {06410DD9-D552-422B-A250-810764497E64}
static const GUID MEDIASUBTYPE_HVC1 =
{ 0x6410dd9, 0xd552, 0x422b, { 0xa2, 0x50, 0x81, 0x7, 0x64, 0x49, 0x7e, 0x64 } };
#endif

#if 1
/// Format header for MEDIASUBTYPE_MM1
// {2164DDFF-9639-4649-8B49-149634C8C252}
static const GUID FORMAT_InfoMM1 = 
{ 0x2164ddff, 0x9639, 0x4649, { 0x8b, 0x49, 0x14, 0x96, 0x34, 0xc8, 0xc2, 0x52 } };

/// Format header for MEDIASUBTYPE_MM2
// {0D041102-EC58-489C-9789-7884FCF95CB6}
static const GUID FORMAT_InfoMM2 =
{ 0xd041102, 0xec58, 0x489c, { 0x97, 0x89, 0x78, 0x84, 0xfc, 0xf9, 0x5c, 0xb6 } };

// Media type for H263 switched multiplexed media frame
// {EDF214E2-F179-46ca-88C9-1CDBA3CF0EDC}
static const GUID MEDIASUBTYPE_MM1 =
{ 0xedf214e2, 0xf179, 0x46ca, { 0x88, 0xc9, 0x1c, 0xdb, 0xa3, 0xcf, 0xe, 0xdc } };

// {031FF60B-75BD-4BC4-8896-13176234EFD6}
static const GUID MEDIASUBTYPE_MM2 =
{ 0x31ff60b, 0x75bd, 0x4bc4, { 0x88, 0x96, 0x13, 0x17, 0x62, 0x34, 0xef, 0xd6 } };

#endif

/// MMF media: generic multiplexed media format
// {D89B678F-9AC9-4E2A-9BF8-AA2D6149DDEF}
static const GUID MEDIASUBTYPE_MMF =
{ 0xd89b678f, 0x9ac9, 0x4e2a, { 0x9b, 0xf8, 0xaa, 0x2d, 0x61, 0x49, 0xdd, 0xef } };
// {43C516E0-C560-4593-925C-393229A5C0C6}
static const GUID FORMAT_InfoMMF =
{ 0x43c516e0, 0xc560, 0x4593, { 0x92, 0x5c, 0x39, 0x32, 0x29, 0xa5, 0xc0, 0xc6 } };

#if 0
/// RGB media with advert information header: RGB24/32 can be determined from standard video header
// {ED52B6B8-5719-4d98-B9AA-F55E04B387AF}
static const GUID MEDIASUBTYPE_RGB24_ADVERT = 
{ 0xed52b6b8, 0x5719, 0x4d98, { 0xb9, 0xaa, 0xf5, 0x5e, 0x4, 0xb3, 0x87, 0xaf } };

// {512C4E24-A4BE-43ea-A293-EAB756592EB6}
static const GUID MEDIASUBTYPE_RGB32_ADVERT = 
{ 0x512c4e24, 0xa4be, 0x43ea, { 0xa2, 0x93, 0xea, 0xb7, 0x56, 0x59, 0x2e, 0xb6 } };

/// YUV Media with advert information header
// {8B68EBEE-713A-4a45-94EA-AD8B560B0CE7}
static const GUID MEDIASUBTYPE_YUV420P_ADVERT = 
{ 0x8b68ebee, 0x713a, 0x4a45, { 0x94, 0xea, 0xad, 0x8b, 0x56, 0xb, 0xc, 0xe7 } };

// H263 Media with advert information header
// {5FCA7AC5-024D-4c1f-B000-392885B976C6}
static const GUID MEDIASUBTYPE_H263M_ADVERT = 
{ 0x5fca7ac5, 0x24d, 0x4c1f, { 0xb0, 0x0, 0x39, 0x28, 0x85, 0xb9, 0x76, 0xc6 } };

// H264 Media with advert information header
// {9C3751AA-C587-4557-B05E-10693734B770}
static const GUID MEDIASUBTYPE_H264M_ADVERT = 
{ 0x9c3751aa, 0xc587, 0x4557, { 0xb0, 0x5e, 0x10, 0x69, 0x37, 0x34, 0xb7, 0x70 } };

// Media Type for H263 switch media with advert header
// {2A9B83C5-7F5C-4d94-9FD6-AEC7CEC30BDA}
static const GUID MEDIASUBTYPE_H263S_ADVERT =
{ 0x2a9b83c5, 0x7f5c, 0x4d94, { 0x9f, 0xd6, 0xae, 0xc7, 0xce, 0xc3, 0xb, 0xda } };

// {7153A1B8-7F3E-424F-9055-308F81B0CE44}
static const GUID MEDIASUBTYPE_H264S_ADVERT =
{ 0x7153a1b8, 0x7f3e, 0x424f, { 0x90, 0x55, 0x30, 0x8f, 0x81, 0xb0, 0xce, 0x44 } };

// Media type for H263 switched multiplexed media frame with advert header
// {301D6F34-5803-4062-B92E-86CED9C9982E}
static const GUID MEDIASUBTYPE_MM1_ADVERT =
{ 0x301d6f34, 0x5803, 0x4062, { 0xb9, 0x2e, 0x86, 0xce, 0xd9, 0xc9, 0x98, 0x2e } };

#endif

// Media Type for H263 switch media
// {107023BC-115C-4c53-92E7-E13FF65BA1B0}
static const GUID MEDIASUBTYPE_H263S = 
{ 0x107023bc, 0x115c, 0x4c53, { 0x92, 0xe7, 0xe1, 0x3f, 0xf6, 0x5b, 0xa1, 0xb0 } };

// Media Type for H264 switch media
// {0E17B98E-275E-47a4-A884-6DB9B3655F2B}
static const GUID MEDIASUBTYPE_H264S = 
{ 0xe17b98e, 0x275e, 0x47a4, { 0xa8, 0x84, 0x6d, 0xb9, 0xb3, 0x65, 0x5f, 0x2b } };

// TODO: LJ: copied from amr_types.h ... rather include it somewhere?
static const GUID MEDIASUBTYPE_AMR =
{ 0x726D6173, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// VORBIS from xiph
// {8A0566AC-42B3-4AD9-ACA3-93B906DDF98A}
static const GUID MEDIASUBTYPE_VORBIS =
{ 0x8a0566ac, 0x42b3, 0x4ad9, { 0xac, 0xa3, 0x93, 0xb9, 0x06, 0xdd, 0xf9, 0x8a } };

// {8D2FD10B-5841-4a6b-8905-588FEC1ADED9}
static const GUID MEDIASUBTYPE_VORBIS2 =
{ 0x8D2FD10B, 0x5841, 0x4a6b, { 0x89, 0x05, 0x58, 0x8F, 0xEC, 0x1A, 0xDE, 0xD9 } };

// {7625AE3A-9DF0-42E0-BDB9-6B872D182517}
static const GUID MEDIASUBTYPE_RTVC_VORBIS = 
{ 0x7625ae3a, 0x9df0, 0x42e0, { 0xbd, 0xb9, 0x6b, 0x87, 0x2d, 0x18, 0x25, 0x17 } };

// {44E04F43-58B3-4de1-9BAA-8901F852DAE4}
static const GUID FORMAT_VORBIS =
{ 0x44e04f43, 0x58b3, 0x4de1, { 0x9b, 0xaa, 0x89, 0x1, 0xf8, 0x52, 0xda, 0xe4 } };

// {B36E107F-A938-4387-93C7-55E966757473}    
static const GUID FORMAT_VORBIS2 =
{ 0xB36E107F, 0xA938, 0x4387, { 0x93, 0xC7, 0x55, 0xE9, 0x66, 0x75, 0x74, 0x73 } };

// {44E04F43-58B3-4de1-9BAA-8901F852DAE4}
// {4DB50C25-F0B1-472A-8F15-7F0D9DA1CADB}
static const GUID FORMAT_RTVC_VORBIS = 
{ 0x4db50c25, 0xf0b1, 0x472a, { 0x8f, 0x15, 0x7f, 0xd, 0x9d, 0xa1, 0xca, 0xdb } };

// Taken from Monogram
// {000000FF-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_AAC =
{ 0x000000ff, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// Taken from Monogram
// {000001FF-0000-0010-8000-00AA00389B71}
static const GUID MEDIASUBTYPE_LATM_AAC =
{ 0x000001ff, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// Taken from Monogram
// {00001602-0000-0010-8000-00aa00389b71}
static const GUID MEDIASUBTYPE_LATM_AAC_2 =
{ 0x00001602, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

// Taken from Monogram
// {1365BE7A-C86A-473C-9A41-C0A6E82C9FA3}
static const GUID CLSID_MediaPlayerClassicDemux =
{ 0x1365BE7A, 0xC86A, 0x473C, { 0x9A, 0x41, 0xC0, 0xA6, 0xE8, 0x2C, 0x9F, 0xA3 } };

