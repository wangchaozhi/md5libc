#ifndef MD5UTILS_H
#define MD5UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include <jni.h>

EXPORT int calculate_md5_from_array(const char** filenames, int count, char* result_buffer);
JNIEXPORT jint JNICALL Java_jni_MD5Utils_calculateMd5FromArray(JNIEnv *env, jclass cls, jobject filenames, jint count, jobject resultBuffer);

#ifdef __cplusplus
}
#endif

#endif // MD5UTILS_H
