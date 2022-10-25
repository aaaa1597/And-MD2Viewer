#include <jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include "NativeSurface.h"

NativeSurface::NativeSurface() : mThreadId(-1) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
}

NativeSurface::~NativeSurface() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
}

/* moveコンストラクタ */
NativeSurface::NativeSurface(NativeSurface &&surface) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
}

///* copyコンストラクタ */
//NativeSurface::NativeSurface(const NativeSurface &surface) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
//}

