#include <jni.h>
#include <map>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>
#include "NativeSurface.h"

std::map<int, NativeSurface> gSufaces = {};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceCreated(JNIEnv *pEnv, jclass /*clazz*/, jint id, jobject surface) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d gSufaces.size=%d %s %s(%d)", id, gSufaces.size(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gSufaces.emplace(id, NativeSurface());
    /* SurfaceViewのNativeWindowを取得 */
    ANativeWindow *pWindow = ANativeWindow_fromSurface(pEnv, surface);
    gSufaces.at(id).surfaceCreated(pWindow);
}

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceChanged(JNIEnv */*env*/, jclass /*clazz*/, jint id, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d %s %s(%d)", id, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gSufaces.at(id).surfaceChanged(width, height);
}

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceDestroyed(JNIEnv */*env*/, jclass /*clazz*/, jint id) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d %s %s(%d)", id, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gSufaces.at(id).surfaceDestroyed();
    gSufaces.erase(id);
}

#ifdef __cplusplus
}
#endif
