cmake_minimum_required(VERSION 3.26)
project(md5libc C)

# 设置 C 语言标准
set(CMAKE_C_STANDARD 11)

# 设置输出为共享库（动态库）
add_library(md5libc SHARED md5utils.c)

# 添加 JNI 头文件路径
# 请根据你的JDK路径修改
include_directories("D:/Program Files/Java/jdk-17.0.11/include")
include_directories("D:/Program Files/Java/jdk-17.0.11/include/win32")

# Windows 平台下指定 DLL 输出
if (WIN32)
    set_target_properties(md5libc PROPERTIES OUTPUT_NAME "md5utils")
endif()

# 链接 OpenSSL 库（适用于 Linux/Unix）
find_package(OpenSSL REQUIRED)
target_link_libraries(md5libc PRIVATE OpenSSL::Crypto)

# 安装目标库到 ./build/lib
install(TARGETS md5libc DESTINATION ${CMAKE_BINARY_DIR}/lib)
