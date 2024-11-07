#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT int calculate_md5_from_array(const char** filenames, int count, char* result_buffer) {
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    unsigned char md5_result[EVP_MAX_MD_SIZE];
    unsigned int md5_length;

    if (!md_ctx) {
        return -1; // Memory allocation error
    }

    for (int i = 0; i < count; ++i) {
        FILE* file = fopen(filenames[i], "rb");
        if (!file) {
            EVP_MD_CTX_free(md_ctx);
            return -1; // File open error
        }

        if (EVP_DigestInit_ex(md_ctx, EVP_md5(), NULL) != 1) {
            fclose(file);
            EVP_MD_CTX_free(md_ctx);
            return -1; // Initialization error
        }

        unsigned char data[1024];
        size_t bytes;
        while ((bytes = fread(data, 1, sizeof(data), file)) != 0) {
            if (EVP_DigestUpdate(md_ctx, data, bytes) != 1) {
                fclose(file);
                EVP_MD_CTX_free(md_ctx);
                return -1; // Update error
            }
        }
        fclose(file);

        if (EVP_DigestFinal_ex(md_ctx, md5_result, &md5_length) != 1) {
            EVP_MD_CTX_free(md_ctx);
            return -1; // Finalize error
        }

        // Convert the MD5 result to a hexadecimal string and store it in result_buffer
        for (unsigned int j = 0; j < md5_length; ++j) {
            sprintf(&result_buffer[(i * md5_length * 2) + (j * 2)], "%02x", md5_result[j]);
        }
    }
    EVP_MD_CTX_free(md_ctx);
    return 0;
}


// JNI适配层 使用 jint 作为返回类型
JNIEXPORT jint JNICALL Java_jni_MD5Utils_calculateMd5FromArray(JNIEnv *env, jclass cls, jobject filenames, jint count, jobject resultBuffer) {
    // Get pointers to ByteBuffer data
    char *file_buffer = (char *)(*env)->GetDirectBufferAddress(env, filenames);
    char *result_buffer = (char *)(*env)->GetDirectBufferAddress(env, resultBuffer);

    if (file_buffer == NULL || result_buffer == NULL) {
        printf("Error: Buffer address is null.\n");
        return -1; // Prevent null pointer error
    }

    // Dynamically allocate memory for file_paths array
    const char **file_paths = (const char **)malloc(count * sizeof(char *));
    if (file_paths == NULL) {
        printf("Error: Memory allocation failed.\n");
        return -1;
    }

    // Parse `file_buffer` into an array of C strings
    char *current = file_buffer;
    for (int i = 0; i < count; i++) {
        file_paths[i] = current;         // Assign pointer to the start of each path
        current += strlen(current) + 1;  // Move to the next path (null-terminated)
    }

    // Call the MD5 calculation function
    int result = calculate_md5_from_array(file_paths, count, result_buffer);

    // Free the dynamically allocated memory
    free(file_paths);

    return (jint)result;
}
