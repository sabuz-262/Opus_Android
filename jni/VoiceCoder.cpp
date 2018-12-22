//
//  VoiceCoder.cpp
//  MobileDialer
//
//  Created by Badsha Shiblo on 3/6/16.
//  Copyright © 2016 REVE Systems. All rights reserved.
//

#include "VoiceCoder.h"
#include <android/log.h>
#define TAG "Opus_JNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG,__VA_ARGS__)


VoiceCoder::VoiceCoder()
{
    lastDecoderInputDataLength=0;
   
    uniteDecodedFrameSize=8*UNITE_FRAME_TIME;
   
     //(bitrate/8) byte rate per sec
    // byterate/1000ms*unitetime
    uniteFrameEncodedSize=(int)((((float)BITRATE/8)/1000.0)*UNITE_FRAME_TIME);
    LOGE("uniteframe decodedsize");
    int err;
    opus_encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, APPLICATION, &err);
    if (err<0)
    {
        LOGE("stderr failed to create an encoder:");
    }
    
    err = opus_encoder_ctl(opus_encoder, OPUS_SET_BITRATE(BITRATE));
    if (err<0)
    {
        LOGE("stderr failed to set bitrate");
        
    }
    int dtx=0;
    dtx=1;//assign dtx as needed.
    
    err = opus_encoder_ctl(opus_encoder, OPUS_SET_DTX(dtx));
    if (err<0)
    {
        LOGE("stderr failed to set DTX ");
        
    }
    
    opus_decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
    if (err<0)
    {
        LOGE("stderr failed to create decoder");
        
    }

   
}
VoiceCoder::~VoiceCoder()
{
    opus_encoder_destroy(opus_encoder);
    opus_decoder_destroy(opus_decoder);
   
}

int VoiceCoder::encode(short* samples,int lenOfSamples, jchar *outputData)
{
  
    
    int nbBytes=1;
    int payloadLength=0;
   
    int remainingSamples=lenOfSamples;
    int sampleCursor=0;
    //unidecoded 240 (30 ms)
    
    while (remainingSamples>0)
    {
       
        if(remainingSamples>=uniteDecodedFrameSize)
        {
	    LOGE("in remainingSamples>=uniteDecodedFrameSize");
            nbBytes = opus_encode(opus_encoder,(opus_int16*)(samples+sampleCursor),uniteDecodedFrameSize,(unsigned char*)(outputData+payloadLength+1),uniteFrameEncodedSize);
            sampleCursor+=uniteDecodedFrameSize;
            
            outputData[payloadLength]=nbBytes;
            //printf("in if nBytesCursor=%d bBytes from enc=%d\n",payloadLength,nbBytes);
            payloadLength+=(nbBytes+1);
            
            remainingSamples-=uniteDecodedFrameSize;
            
        }else
        {
	    LOGE("in remainingSamples>=uniteDecodedFrameSize else");
            nbBytes = opus_encode(opus_encoder,(opus_int16*)(samples+sampleCursor),remainingSamples,(unsigned char*)(outputData+payloadLength+1),uniteFrameEncodedSize);
            
            sampleCursor+=remainingSamples;
            outputData[payloadLength]=nbBytes;
            //printf("in else nBytesCursor=%d bBytes from enc=%d\n",payloadLength,nbBytes);
            payloadLength+=(nbBytes+1);
            remainingSamples-=uniteDecodedFrameSize;
            return payloadLength;
        }
        
    }
    return  payloadLength;
    
}

int VoiceCoder::decode(jchar* inputData,int length,short *outputSample)
{
    int frame_size=0;
    int lengthCursor=0;
    int nBytes;
    int resultLength=0;
    int remainingLength=length;
    lastDecoderInputDataLength=length;
    //short outputSamples[];
    //int i=0;
    while (remainingLength>0)
    {
        nBytes=inputData[lengthCursor];
        if(nBytes!=0)
        {
            //printf("input data cursor=%d nBytes=%d remaining length=%d\n",lengthCursor+1,nBytes,remainingLength);
            frame_size = opus_decode(opus_decoder, (unsigned char*)(inputData+lengthCursor+1), nBytes, ( opus_int16*) (outputSample+resultLength),6*80, 0);
            if (frame_size<0)
            {
                fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
                
               // return 0;
            }else{
                //printf("Decoded frame size=%d\n",frame_size);
                resultLength+=frame_size;
            }
        }
        remainingLength-=(1+nBytes);
        lengthCursor+=(nBytes+1);  //[0]=1+8+ [9]=1+9+[19]1+10
       // i++;
    }
    
    return resultLength;

}
int VoiceCoder::decodeLostPacket(short *outputSample)
{
//    int frame_size=0;
//    int lengthCursor=0;
//    
//    int resultLength=0;
//    int remainingLength=lastDecoderInputDataLength;
//    //short outputSamples[];
//    //int i=0;
//    while (remainingLength>0)
//    {
//        
//        
//            frame_size = opus_decode(opus_decoder,NULL,0, ( opus_int16*) (outputSample+resultLength),6*80, 0);
//            if (frame_size<0)
//            {
//                fprintf(stderr, "decoder failed: %s\n", opus_strerror(frame_size));
//                
//                // return 0;
//            }else{
//                //printf("Decoded frame size=%d\n",frame_size);
//                resultLength+=frame_size;
//            }
//       
//            remainingLength-=(1+nBytes);
//            lengthCursor+=(nBytes+1);  //[0]=1+8+ [9]=1+9+[19]1+10
//        // i++;
//    }
//    
//    return resultLength;

}
void VoiceCoder::resetEncoderContext()
{
    opus_encoder_init(opus_encoder,SAMPLE_RATE, 1, APPLICATION);
   
}
void VoiceCoder::resetDecoderContext()
{
  
    opus_decoder_init(opus_decoder, SAMPLE_RATE, 1);
}

