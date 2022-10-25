#include <jni.h>
#include <map>
#include <string>
#include <android/log.h>
#include "NativeSurface.h"

std::map<int, NativeSurface> gSufaces = {};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceCreated(JNIEnv */*env*/, jclass /*clazz*/, jint id, jobject surface) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d gSufaces.size=%d %s %d %s", id, gSufaces.size(),__PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
    gSufaces.emplace(id, NativeSurface());
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d gSufaces.size=%d %s %d %s", id, gSufaces.size(),__PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
}

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceChanged(JNIEnv */*env*/, jclass /*clazz*/, jint id, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d %s %d %s", id, __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
}

JNIEXPORT void JNICALL Java_com_tks_md2viewer_NativeLibIf_surfaceDestroyed(JNIEnv */*env*/, jclass /*clazz*/, jint id) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "id=%d %s %d %s", id, __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
    gSufaces.erase(id);
}

#ifdef __cplusplus
}
#endif
