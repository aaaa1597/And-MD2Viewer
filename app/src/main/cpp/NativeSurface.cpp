#include <jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include "NativeSurface.h"

NativeSurface::NativeSurface() : mThreadId(-1) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

NativeSurface::~NativeSurface() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

/* moveコンストラクタ */
NativeSurface::NativeSurface(NativeSurface &&surface) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

///* copyコンストラクタ */
//NativeSurface::NativeSurface(const NativeSurface &surface) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
//}

void NativeSurface::surfaceCreated(ANativeWindow *pWindow) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mpWindow = pWindow;
}

void NativeSurface::surfaceChanged() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

void NativeSurface::surfaceDestroyed() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    ANativeWindow_release(mpWindow);
    mpWindow = NULL;
}
