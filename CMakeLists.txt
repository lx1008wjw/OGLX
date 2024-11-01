cmake_minimum_required(VERSION 3.10)
project(OGLX)

file(GLOB_RECURSE  SOURCES src/*.c)
file(GLOB_RECURSE  EXTRA_SRC extras/utils/src/*.c)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${EXTRA_SRC})
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extras/utils/include>
        $<INSTALL_INTERFACE:include> # 安装时包含路径
        $<INSTALL_INTERFACE:extras/utils/include> # 安装时包含路径
)
target_include_directories(${PROJECT_NAME} PRIVATE src/sgl)
target_include_directories(${PROJECT_NAME} PRIVATE src/mth)

