#include <jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>
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
    EGLint major, minor;
    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(mEGLDisplay, &major, &minor);

    /* 設定取得 */
    const EGLint configAttributes[] = {
            EGL_LEVEL, 0,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_ALPHA_SIZE, EGL_OPENGL_BIT,
            /*EGL_BUFFER_SIZE, 32 */  /* ARGB8888用 */
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(mEGLDisplay, configAttributes, &config, 1, &numConfigs);

    /* context生成 */
    const EGLint contextAttributes[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    mEGLContext = eglCreateContext(mEGLDisplay, config, EGL_NO_CONTEXT, contextAttributes);

    /* ウィンドウバッファサイズとフォーマットを設定 */
    EGLint format;
    eglGetConfigAttrib(mEGLDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow_setBuffersGeometry(mpWindow, 0, 0, format);

    /* surface生成 */
    mEGLSurface = eglCreateWindowSurface(mEGLDisplay, config, mpWindow, nullptr);
    if(mEGLSurface == EGL_NO_SURFACE) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "surface生成 失敗!!");
        return;
    }

    /* context再生成 */
    mEGLContext = eglCreateContext(mEGLDisplay, config, EGL_NO_CONTEXT, contextAttributes);

    /* 作成したsurface/contextを関連付け */
    if(eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext) == EGL_FALSE) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "surface/contextの関連付け 失敗!!");
        return;
    }

    /* チェック */
    EGLint w,h;
    eglQuerySurface(mEGLDisplay, mEGLSurface, EGL_WIDTH, &w);
    eglQuerySurface(mEGLDisplay, mEGLSurface, EGL_HEIGHT,&h);
    glViewport(0,0,w,h);
}

void NativeSurface::initGL() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mProgram = createProgram(VERTEXSHADER, FRAGMENTSHADER);
}

GLuint NativeSurface::createProgram(const char *vertexshader, const char *fragmentshader) {
    GLuint vhandle = loadShader(GL_VERTEX_SHADER, vertexshader);
    if(vhandle == GL_FALSE) return GL_FALSE;

    GLuint fhandle = loadShader(GL_FRAGMENT_SHADER, fragmentshader);
    if(fhandle == GL_FALSE) return GL_FALSE;

    GLuint programhandle = glCreateProgram();
    if(programhandle == GL_FALSE) {
        checkGlError("glCreateProgram");
        return GL_FALSE;
    }

    glAttachShader(programhandle, vhandle);
    checkGlError("glAttachShader(VERTEX_SHADER)");
    glAttachShader(programhandle, fhandle);
    checkGlError("glAttachShader(FRAGMENT_SHADER)");

    glLinkProgram(programhandle);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(programhandle, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE) {
        GLint bufLen = 0;
        glGetProgramiv(programhandle, GL_INFO_LOG_LENGTH, &bufLen);
        if(bufLen) {
            char *logstr = (char*)malloc(bufLen);
            glGetProgramInfoLog(mProgram, bufLen, nullptr, logstr);
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "glLinkProgram() Fail!!\n %s", logstr);
            free(logstr);
        }
        glDeleteProgram(programhandle);
        programhandle = GL_FALSE;
    }

    return programhandle;
}

GLuint NativeSurface::loadShader(int shadertype, const char *sourcestring) {
    GLuint shaderhandle = glCreateShader(shadertype);
    if(!shaderhandle) return GL_FALSE;

    glShaderSource(shaderhandle, 1, &sourcestring, nullptr);
    glCompileShader(shaderhandle);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen) {
            char *logbuf = (char*)malloc(infoLen);
            if(logbuf) {
                glGetShaderInfoLog(shaderhandle, infoLen, nullptr, logbuf);
                __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "shader failuer!!\n%s", logbuf);
                free(logbuf);
            }
        }
        glDeleteShader(shaderhandle);
        shaderhandle = GL_FALSE;
    }

    return shaderhandle;
}

void NativeSurface::checkGlError(const char *argstr) {
    for(GLuint error = glGetError(); error; error = glGetError())
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "after %s errcode=%d", argstr, error);
}

void NativeSurface::predrawGL() {
    GLuint ma_PositionHandle = glGetAttribLocation(mProgram, "vPosition");
    mu_rotMatrixHandle = glGetUniformLocation(mProgram, "u_rotMatrix");

    glUseProgram(mProgram);
    static const GLfloat vertexes[] = {0,0.5, -0.5,-0.5, 0.5,-0.5};
    glVertexAttribPointer(ma_PositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertexes);
    glEnableVertexAttribArray(ma_PositionHandle);

    mxPos = 0;
    myPos = 0;
    glClearColor(0, 0.5, 0, 0.5);
}

void NativeSurface::drawGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mDspW != 0 && mDspH != 0) {
        mxPos += mMoveX;
        myPos += mMoveY;
        if((mxPos > (2*mDspW)) || (mxPos < 0)) mMoveX = -mMoveX;
        if((myPos > (2*mDspH)) || (myPos < 0)) mMoveY = -mMoveY;
        float translateMatrix[] = {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                mxPos/mDspW-1, myPos/mDspH-1, 1, 1
        };
        glUniformMatrix4fv(mu_rotMatrixHandle, 1, false, translateMatrix);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    eglSwapBuffers(mEGLDisplay, mEGLSurface);
}

void NativeSurface::finEGL() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "s %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    ANativeWindow_release(mpWindow);
    mpWindow = nullptr;
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "e %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}

void NativeSurface::destroy() {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "s %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    mDrwThread->join();
    mDrwThread.reset();
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "e %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
}
