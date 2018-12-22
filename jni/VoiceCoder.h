//
//  VoiceCoder.hpp
//  MobileDialer
//
//  Created by Badsha Shiblo on 3/6/16.
//  Copyright © 2016 REVE Systems. All rights reserved.
//

#ifndef VoiceCoder_h
#define VoiceCoder_h
#include <stdio.h>
#include <opus.h>
#include <opus_defines.h>
#include <opus_custom.h>
#include <opus_types.h>
#include <jni.h>

#define USE_OPUS

#define SAMPLE_RATE 8000

#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_VOIP
#define UNITE_FRAME_TIME 20        //value must be less than or equal to 30 ms

#define BITRATE 16000

class VoiceCoder
{
    OpusEncoder *opus_encoder;
    OpusDecoder *opus_decoder;
    
    int uniteFrameEncodedSize;
    int uniteDecodedFrameSize;
    int lastDecoderInputDataLength;
    public:
    VoiceCoder();// 
    ~VoiceCoder();
    void resetEncoderContext();
    void resetDecoderContext();
    //retuns length of the compressed data
    //lenOfSamples has to be multiples of 80 (80 represent 10 ms voice data at sample rate 8000) 
    int encode(short *samples,int lenOfSamples, jchar *outputData);
    //returns length of the decompressed samples
    int decode(jchar* inputData,int length,short *outputSample);
    int decodeLostPacket(short *outputSample);
};
#endif /* VoiceCoder_hpp */
