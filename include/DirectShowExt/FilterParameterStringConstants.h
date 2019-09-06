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

// filter level parameters
#define FILTER_PARAM_WIDTH                    "width"
#define FILTER_PARAM_HEIGHT                   "height"
#define FILTER_PARAM_FPS                      "fps"
#define FILTER_PARAM_MEDIA_ID                 "media_id"
#define FILTER_PARAM_GROUP_ID                 "group_id"

#define FILTER_PARAM_ORIENTATION			        "orientation"
#define FILTER_PARAM_ROTATION_MODE		        "rotationmode"
#define FILTER_PARAM_SUB_PICTURE_POSITION	    "position"

#define FILTER_PARAM_TARGET_WIDTH             "target_width"
#define FILTER_PARAM_TARGET_HEIGHT            "target_height"
#define FILTER_PARAM_SUB_PIC_WIDTH            "sub_pic_width"
#define FILTER_PARAM_SUB_PIC_HEIGHT           "sub_pic_height"
#define FILTER_PARAM_OFFSET_X                 "offset_x"
#define FILTER_PARAM_OFFSET_Y                 "offset_y"
#define FILTER_PARAM_BORDER_WIDTH             "border_width"

#define FILTER_PARAM_MODE                     "mode"
#define FILTER_PARAM_VPS                      "vps"
#define FILTER_PARAM_SPS                      "sps"
#define FILTER_PARAM_PPS                      "pps"
#define FILTER_PARAM_USE_MS_H264              "usems264"
#define FILTER_PARAM_H264_OUTPUT_TYPE         "h264_output_type"

#define FILTER_PARAM_TARGET_BITRATE_KBPS      "target_bitrate_kbps"

#define FILTER_PARAM_START_CHANNEL            "start_channel"

#define FILTER_PARAM_SKIP_FRAME               "skip_frame"
#define FILTER_PARAM_TOTAL_FRAMES             "total_frames"
#define FILTER_PARAM_SOURCE_FRAMERATE         "source_framerate"
#define FILTER_PARAM_TARGET_FRAMERATE         "target_framerate"
#define FILTER_PARAM_SKIP_X_EVERY_Y           "skip_x_every_y"
#define FILTER_PARAM_TARGET_RATE_BASED        "target_rate_based"

#define FILTER_PARAM_TOP_CROP                 "top"
#define FILTER_PARAM_BOTTOM_CROP              "bottom"
#define FILTER_PARAM_LEFT_CROP                "left"
#define FILTER_PARAM_RIGHT_CROP               "right"

// H263/H264-specific codec parameters
#define CODEC_PARAM_IN_COLOUR                 "incolour"
#define CODEC_PARAM_OUT_COLOUR                "outcolour"
#define CODEC_PARAM_MODE_OF_OPERATION         "mode of operation"
#define CODEC_PARAM_FRAME_BIT_LIMIT           "framebitlimit"	
#define CODEC_PARAM_NOTIFYONIFRAME            "notifyoniframe"
#define CODEC_PARAM_SWITCH_FRAME_PERIOD       "switchframeperiod"
#define	CODEC_PARAM_I_PICTURE_MULTIPLIER      "ipicturemultiplier"
#define	CODEC_PARAM_AUTO_IFRAME_DETECT_FLAG   "autoiframedetectflag"
#define CODEC_PARAM_IFRAME_PERIOD             "iframeperiod"
#define CODEC_PARAM_QUALITY                   "quality"
// Rate control-related parameters
#define CODEC_PARAM_RATE_CONTROL_MODEL_TYPE   "rate control model type"
#define CODEC_PARAM_MAX_BITS_PER_FRAME        "max bits per frame"
#define CODEC_PARAM_NUM_RATE_CONTROL_FRAMES   "num rate control frames"
#define CODEC_PARAM_MAX_DISTORTION            "max distortion"
#define CODEC_PARAM_IPICTURE_DMAX_MULTIPLIER  "ipicture dmax multiplier"
#define CODEC_PARAM_IPICTURE_DMAX_FRACTION    "ipicture dmax fraction"
#define CODEC_PARAM_MINIMUM_INTRA_QP          "minimum intra qp"
#define CODEC_PARAM_MINIMUM_INTER_QP          "minimum inter qp"
#define CODEC_PARAM_RATE_OVERSHOOT_PERCENT    "rate overshoot percent"
