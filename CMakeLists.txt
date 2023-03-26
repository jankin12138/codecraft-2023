project(CodeCraftSDK)
cmake_minimum_required (VERSION 3.8)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
# 将 CMAKE_BUILD_TYPE 设置为 Debug 模式
set(CMAKE_BUILD_TYPE Debug)
if (!WIN32)
    link_libraries(pthread rt m)
endif (!WIN32)

AUX_SOURCE_DIRECTORY(. src)
ADD_EXECUTABLE(main ${src} RobotCrashPreventer.cpp)