LOCAL_PATH := $(call my-dir)
OPUS_DIR            := opus-1.1.2

include $(LOCAL_PATH)/opus-1.1.2/Android.mk

include $(CLEAR_VARS)

LOCAL_MODULE        := codec
LOCAL_SRC_FILES     := VoiceCoder.cpp JNIWrapper.cpp
LOCAL_LDLIBS        := -lm -llog -lz
LOCAL_C_INCLUDES    := $(LOCAL_PATH)/$(OPUS_DIR)/include
LOCAL_SHARED_LIBRARIES := opus
include $(BUILD_SHARED_LIBRARY)
