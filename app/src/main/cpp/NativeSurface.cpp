#include <jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "NativeSurface.h"

/** コンストラクタ/デストラクタ ******************************************************************************/
/* コンストラクタ */
NativeSurface::NativeSurface() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

/* デストラクタ */
NativeSurface::~NativeSurface() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

/* moveコンストラクタ */
NativeSurface::NativeSurface(NativeSurface &&surface) noexcept {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

///* copyコンストラクタ */
//NativeSurface::NativeSurface(const NativeSurface &surface) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %d %s", __PRETTY_FUNCTION__, __LINE__, __FILE_NAME__);
//}

/** SurfaceViewイベント **********************************************************************************/
/* SurfaceViewイベント(create) */
void NativeSurface::surfaceCreated(ANativeWindow *pWindow) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mpWindow = pWindow;
    /* 描画スレッド生成 */
    createThread();
}

/* SurfaceViewイベント(changed) */
void NativeSurface::surfaceChanged(int w, int h) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mDspW = w;
    mDspH = h;
    isSurfaceChanged = true;
}

/* SurfaceViewイベント(destroyed) */
void NativeSurface::surfaceDestroyed() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mStatus = Status::FINISH;
    destroy();
}

/** 個別関数 *********************************************************************************************/
/* スレッド生成 */
void NativeSurface::createThread() {
    mStatus = Status::INITIALIZING;
    mDrwThread = std::make_unique<std::thread>(NativeSurface::draw_thread, this);
}

/* 描画スレッド */
void NativeSurface::draw_thread(NativeSurface *pSurface) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    if(pSurface == nullptr) return;

    pSurface->initEGL();
    pSurface->initGL();
    pSurface->mStatus = Status::DRAWING;

    pSurface->predrawGL();
    struct timespec req = {0, (int)(16.66*1000000)};
    for(;pSurface->mStatus==Status::DRAWING;) {

        /* SurfaceCreated()が動作した時は、画面サイズ変更を実行 */
        if(pSurface->isSurfaceChanged) {
            pSurface->isSurfaceChanged = false;
            glViewport(0, 0, pSurface->mDspW, pSurface->mDspH);
        }

        /* 通常の描画処理 */
        pSurface->drawGL();
        nanosleep(&req, nullptr);
    }

    pSurface->finEGL();

    return;
}

void NativeSurface::initEGL() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

void NativeSurface::initGL() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

void NativeSurface::predrawGL() {
//    GLuint ma_PositionHandle = glGetAttribLocation(mProgram, "vPosition");
//    mu_rotMatrixHandle = glGetUniformLocation(mProgram, "u_rotMatrix");
//
//    glUseProgram(mProgram);
//    static const GLfloat vertexes[] = {0,0.5, -0.5,-0.5, 0.5,-0.5};
//    glVertexAttribPointer(ma_PositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertexes);
//    glEnableVertexAttribArray(ma_PositionHandle);
//
//    mxPos = 0;
//    myPos = 0;
//    glClearColor(0, 0.5, 0, 0.5);
}

void NativeSurface::drawGL() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    if(mDspW != 0 && mDspH != 0) {
//        mxPos += mMoveX;
//        myPos += mMoveY;
//        if((mxPos > (2*mDspW)) || (mxPos < 0)) mMoveX = -mMoveX;
//        if((myPos > (2*mDspH)) || (myPos < 0)) mMoveY = -mMoveY;
//        float translateMatrix[] = {
//                1,0,0,0,
//                0,1,0,0,
//                0,0,1,0,
//                mxPos/mDspW-1, myPos/mDspH-1, 1, 1
//        };
//        glUniformMatrix4fv(mu_rotMatrixHandle, 1, false, translateMatrix);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    }
//    eglSwapBuffers(mEGLDisplay, mEGLSurface);
}

void NativeSurface::finEGL() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    ANativeWindow_release(mpWindow);
    mpWindow = nullptr;
}

void NativeSurface::destroy() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mDrwThread->join();
}
