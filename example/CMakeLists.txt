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
###
# if(ALL OR DIR#INDEX#)
#     set(SRC_FILES
#         #INDEX#/main.cpp
#     )
# endif()
###
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
if(ALL OR DIR04)
    set(SRC_FILES
        04/main.cpp
    )
endif()
if(ALL OR DIR05)
    set(SRC_FILES
        05/main.cpp
    )
endif()
if(ALL OR DIR06)
    set(SRC_FILES
        06/main.cpp
    )
endif()
if(ALL OR DIR07)
    set(SRC_FILES
        07/main.cpp
    )
endif()
if(ALL OR DIR08)
    set(SRC_FILES
        08/main.cpp
    )
endif()
if(ALL OR DIR09)
    set(SRC_FILES
        09/main.cpp
    )
endif()
if(ALL OR DIR10)
    set(SRC_FILES
        10/main.cpp
    )
endif()
if(ALL OR DIR11)
    set(SRC_FILES
        11/main.cpp
    )
endif()
if(ALL OR DIR12)
    set(SRC_FILES
        12/main.cpp
    )
endif()
if(ALL OR DIR13)
    set(SRC_FILES
        13/main.cpp
    )
endif()
if(ALL OR DIR14)
    set(SRC_FILES
        14/main.cpp
    )
endif()
###

# 添加新增源文件
list(APPEND SRC_FILES
    src/byte.cpp
    src/format.cpp
    src/rand_int.cpp
)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/common)

# 生成可执行程序
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
