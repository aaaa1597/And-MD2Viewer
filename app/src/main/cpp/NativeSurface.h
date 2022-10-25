#ifndef NATIVESURFACE_H
#define NATIVESURFACE_H

#include <thread>
#include <memory>
#include <android/native_window.h>

class NativeSurface final {
public:
    enum class Status : int {
        NONE,
        INITIALIZING,
        DRAWING,
        FINISH,
    };
    NativeSurface();
    ~NativeSurface();
    NativeSurface(NativeSurface &&surface) noexcept;            /* moveコンストラクタ */
    NativeSurface& operator=(NativeSurface &&surface) = default;/* move =演算子 */
    NativeSurface(const NativeSurface &surface) = delete;           /* copyコンストラクタ */
    NativeSurface& operator=(const NativeSurface &surface) = delete;/* copy  =演算子 */
    /* SurfaveViewイベント */
    void surfaceCreated(ANativeWindow *pWindow);
    void surfaceChanged(int w, int h);
    void surfaceDestroyed();

private:
    void createThread();
    static void draw_thread(NativeSurface *pSurface);
    void initEGL();
    void finEGL();
    void initGL();
    void predrawGL();
    void drawGL();
    void destroy();

private:
    ANativeWindow *mpWindow = nullptr;
    std::unique_ptr<std::thread> mDrwThread;
    Status mStatus = Status::NONE;
    bool isSurfaceChanged = false;
    int mDspW = 0;
    int mDspH = 0;
};

#endif //NATIVESURFACE_H
