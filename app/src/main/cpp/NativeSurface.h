#ifndef NATIVESURFACE_H
#define NATIVESURFACE_H

#include <pthread.h>

class NativeSurface final {
public:
    NativeSurface();
    ~NativeSurface();
    NativeSurface(NativeSurface &&surface);                     /* moveコンストラクタ */
    NativeSurface& operator=(NativeSurface &&surface) = default;/* move =演算子 */
    NativeSurface(const NativeSurface &surface) = delete;           /* copyコンストラクタ */
    NativeSurface& operator=(const NativeSurface &surface) = delete;/* copy  =演算子 */
public:
    pthread_t mThreadId = -1;
};

#endif //NATIVESURFACE_H
