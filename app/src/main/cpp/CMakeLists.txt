cmake_minimum_required(VERSION 3.18.1)

project("md2viewer")
add_library(md2viewer SHARED NativeLibIf.cpp
                             NativeSurface.cpp)
find_library(log-lib log)
target_link_libraries(md2viewer ${log-lib} android EGL GLESv2 m)
