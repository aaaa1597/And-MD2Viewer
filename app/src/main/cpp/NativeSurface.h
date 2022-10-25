#ifndef NATIVESURFACE_H
#define NATIVESURFACE_H

#include <thread>
#include <memory>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
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

private:
    GLuint createProgram(const char *vertexshader, const char *fragmentshader);
    GLuint loadShader(int i, const char *vertexshader);
    void checkGlError(const char *argstr);

private:
    const char *VERTEXSHADER =
            "attribute vec4 vPosition;\n"
            "uniform mat4 u_rotMatrix;\n"
            "void main() {\n"
            "    gl_Position = u_rotMatrix * vPosition;\n"
            "}\n";

    const char *FRAGMENTSHADER =
            "precision mediump float;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(0.0, 1.0, 1.0, 0.7);\n"
            "}\n";
    EGLDisplay mEGLDisplay = nullptr;
    EGLContext mEGLContext = nullptr;
    EGLSurface mEGLSurface = nullptr;
    GLuint mProgram = -1;
    GLuint mu_rotMatrixHandle = -1;

    /* 移動 */
    static const int AMOUNTOFMOVE = -50;
    float mMoveX = AMOUNTOFMOVE;
    float mMoveY = AMOUNTOFMOVE;
    float mxPos = 100;
    float myPos = 130;
};

#endif //NATIVESURFACE_H
