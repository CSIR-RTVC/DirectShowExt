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
#include <string>

/**
* @brief #defines for DirectShow filter parameters.
* Each define is prefixed with a "D_" to avoid clashes
* with const std::string definitions.
* We avoid defining everything as const std::strings
* to not have to link all the filters to the artist project.
*/
const int BYTES_PER_PIXEL_RGB24 = 3;
const int BYTES_PER_PIXEL_RGB32 = 4;
// YUV420 Planar unsigned char 
const double BYTES_PER_PIXEL_YUV420P = 1.5;
// YUV420 Planar Short 
const double BYTES_PER_PIXEL_YUV420P_S = 3;

enum CONCAT_ORIENTATIONS
{
  TOP = 0,
  BOTTOM = 1,
  LEFT = 2,
  RIGHT = 3
};

#define D_IN_COLOUR_RGB24         "0"
#define D_IN_COLOUR_YUV420P       "16"
#define D_IN_COLOUR_YUV420P8      "17"

#define H264_VPP 0
#define H264_H264 1
#define H264_AVC1 2

#define D_MAX_QUALITY             31 // ERROR, double check?
#define D_MAX_QUALITY_H264        51
#define D_MAX_QUALITY_H265        51

#if 0

#define FILTER_PARAM_WIDTH             "width"
#define FILTER_PARAM_HEIGHT            "height"
#define D_QUALITY                 "quality"
#define CODEC_PARAM_IN_COLOUR               "incolour"
#define CODEC_PARAM_OUT_COLOUR              "outcolour"
#define D_TARGET_WIDTH            "targetwidth"
#define FILTER_PARAM_TARGET_HEIGHT           "targetheight"
#define D_TARGET_FILE             "targetfile"
#define D_MODE                    "mode"
#define D_TOP_CROP                "top"
#define D_BOTTOM_CROP             "bottom"
#define D_LEFT_CROP               "left"
#define D_RIGHT_CROP              "right"
#define D_X_FRAMES_PER_SECOND     "xframespersecond"
#define D_FRAMES_PER_X_SECONDS    "framesperxseconds"
#define D_CONCAT_ORIENTATION      "concatorientation"
#define D_MODE_OF_OPERATION       "modeofoperation"
#define D_MODE_OF_OPERATION_H264  "mode of operation"
#define D_FRAME_BIT_LIMIT         "framebitlimit"	
#define D_STARTCHANNEL            "startchannel"
#define D_MEDIA_ID                "media_id"
#define D_NOTIFYONIFRAME          "notifyoniframe"
#define D_SWITCH_FRAME_PERIOD     "switchframeperiod"
#define D_STREAM_USING_TCP        "streamusingtcp"
#define	D_I_PICTURE_MULTIPLIER    "ipicturemultiplier"
#define	D_AUTO_IFRAME_DETECT_FLAG "autoiframedetectflag"
#define D_IFRAME_PERIOD           "iframeperiod"
#define D_USE_MS_H264             "usems264"
#define D_H264_TYPE               "h264type"
#define D_TARGET_BITRATE_KBPS     "target_bitrate_kbps"

// Rate control-related parameters
#define D_MAX_BITS_PER_FRAME        "max bits per frame"
#define D_NUM_RATE_CONTROL_FRAMES   "num rate control frames"
#define D_MAX_DISTORTION            "max distortion"
#define D_IPICTURE_DMAX_MULTIPLIER  "ipicture dmax multiplier"
#define D_IPICTURE_DMAX_FRACTION    "ipicture dmax fraction"
#define D_MINIMUM_INTRA_QP          "minimum intra qp"
#define D_MINIMUM_INTER_QP          "minimum inter qp"
#define D_RATE_OVERSHOOT_PERCENT    "rate overshoot percent"

#define D_ORIENTATION			        "orientation"
#define D_ROTATION_MODE		        "rotationmode"
#define D_SUB_PICTURE_POSITION	  "position"

#define D_GROUP_ID                  "group_id"

#define D_ESTIMATED_VIDEO_BITRATE "estimatedvideobitrate"
#define D_ESTIMATED_FRAMERATE     "estimatedframerate"
#define D_ESTIMATED_BITRATE       "estimatedbitrate"

#define D_TARGET_FILE             "targetfile"

#define D_VIDEO_WIDTH             "width"
#define D_VIDEO_HEIGHT            "height"
#define D_MEDIA_TYPE              "mediatype"
#define D_MEDIA_SUBTYPE           "mediasubtype"

#define D_NUMBER_OF_CHANNELS      "numberofchannels"
#define D_START_CHANNEL           "startchannel"
#define D_SAMPLING_RATE           "samplingrate"
#define D_MAX_FRAME_SIZE          "maxframesize"
#define D_FRAME_BIT_LIMITS        "framebitlimit"
#define D_FILE_NAME               "filename"
#define D_BITS_PER_SAMPLE         "bitspersample"
#define D_SAMPLES_PER_SECOND      "samplespersecond"
#define D_CHANNELS "channels"
#define D_FRAME_RATE "framerate"

#endif
