# 设置最小版本
cmake_minimum_required(VERSION 4.0)

# 设置项目名称
project(mcpp LANGUAGES CXX)

# 设置C++版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置调试模式
set(CMAKE_BUILD_TYPE "Debug")

# 设置源文件
if(ALL OR DIR01)
    set(SRC_FILES
        01/main.cpp
    )
endif()

if(ALL OR DIR02)
    set(SRC_FILES
        02/main.cpp
    )
endif()

if(ALL OR DIR03)
    set(SRC_FILES
        03/main.cpp
    )
endif()

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/common)

# 生成可执行程序
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
