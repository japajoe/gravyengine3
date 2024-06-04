cmake_minimum_required(VERSION 3.5.1)

project(glad)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

include_directories(
    "${PROJECT_SOURCE_DIR}/include"
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})