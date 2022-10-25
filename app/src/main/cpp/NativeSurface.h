#ifndef NATIVESURFACE_H
#define NATIVESURFACE_H

#include <pthread.h>
#include <android/native_window.h>

class NativeSurface final {
public:
    NativeSurface();
    ~NativeSurface();
    NativeSurface(NativeSurface &&surface);                     /* moveコンストラクタ */
    NativeSurface& operator=(NativeSurface &&surface) = default;/* move =演算子 */
    NativeSurface(const NativeSurface &surface) = delete;           /* copyコンストラクタ */
    NativeSurface& operator=(const NativeSurface &surface) = delete;/* copy  =演算子 */
    /* SurfaveViewイベント */
    void surfaceCreated(ANativeWindow *pWindow);
    void surfaceChanged();
    void surfaceDestroyed();

private:
    ANativeWindow *mpWindow = nullptr;
    pthread_t mThreadId = -1;
};

#endif //NATIVESURFACE_H
