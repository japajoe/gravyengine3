cmake_minimum_required(VERSION 3.5.1)

project(imgui)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add sources
file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Specify include Directory
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})