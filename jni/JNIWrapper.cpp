#include <jni.h>
#include <android/log.h>
#include <opus.h>
#include<VoiceCoder.h>

/* Header for class net_abcdefgh_opustrial_codec_Opus */
#ifndef _Included_com_revesoft_audiorecord_OPUSInterface
#define _Included_com_revesoft_audiorecord_OPUSInterface


#define TAG "Opus_JNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN  , TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG,__VA_ARGS__)
#ifdef __cplusplus
extern "C" {
    #endif

#include<stdlib.h>
#include<VoiceCoder.h>
VoiceCoder voiceCoder;
jshort* inputData;
jchar* encoded;
jchar* decoded;
jshort* outputData;

    JNIEXPORT void JNICALL Java_com_revesoft_audiorecord_OPUSInterface_open
    (JNIEnv* env, jobject obj, jint lenOfSamples){

	inputData = (jshort*) malloc(lenOfSamples * sizeof(jshort));
	encoded = (jchar *) malloc(lenOfSamples * sizeof(jchar));
	decoded = (jchar *) malloc(lenOfSamples * sizeof(jchar));
	outputData = (jshort*) malloc(lenOfSamples * sizeof(jshort));
    }
    JNIEXPORT jint JNICALL Java_com_revesoft_audiorecord_OPUSInterface_encode
    (JNIEnv* env, jobject obj, jshortArray samples, jint lenOfSamples, jcharArray outputData) {
       
	env->GetShortArrayRegion(samples, 0, lenOfSamples, inputData);
	int encodedLen = voiceCoder.encode(inputData, lenOfSamples, encoded);
	env->SetCharArrayRegion(outputData, 0, encodedLen, encoded);
	return encodedLen;
    }
   JNIEXPORT jint JNICALL Java_com_revesoft_audiorecord_OPUSInterface_decode
    (JNIEnv* env, jobject obj, jcharArray inputData, jint length, jshortArray outputSample) {
  	env->GetCharArrayRegion(inputData, 0, length, decoded);
	int decodedLen = voiceCoder.decode(decoded, length, outputData);
	env->SetShortArrayRegion(outputSample, 0, decodedLen, outputData);
	return decodedLen;
    }
    JNIEXPORT void JNICALL Java_com_revesoft_audiorecord_OPUSInterface_close
    (JNIEnv* env, jobject obj){
        
    }
    #ifdef __cplusplus
}
#endif
#endif
