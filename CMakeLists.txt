cmake_minimum_required(VERSION 3.12)

project(gravyengine3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#Currently only working for linux
option(ENABLE_ASSIMP "Enable Assimp" ON)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

include_directories(
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/include/Audio"
    "${PROJECT_SOURCE_DIR}/include/Audio/DSP"
    "${PROJECT_SOURCE_DIR}/include/Core"
    "${PROJECT_SOURCE_DIR}/include/Embedded"
    "${PROJECT_SOURCE_DIR}/include/External"
    "${PROJECT_SOURCE_DIR}/include/External/glad"
    "${PROJECT_SOURCE_DIR}/include/External/GLFW"
    "${PROJECT_SOURCE_DIR}/include/External/imgui"
    "${PROJECT_SOURCE_DIR}/include/External/miniaudioex"
    "${PROJECT_SOURCE_DIR}/include/Physics"
    "${PROJECT_SOURCE_DIR}/include/Rendering"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Buffers"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Materials"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Shaders"
    "${PROJECT_SOURCE_DIR}/include/System"
    "${PROJECT_SOURCE_DIR}/include/System/Collections"
    "${PROJECT_SOURCE_DIR}/include/System/Drawing"
    "${PROJECT_SOURCE_DIR}/include/System/IO"
    "${PROJECT_SOURCE_DIR}/include/System/Numerics"
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(GRAVY_DEBUG_LOGGING)
else()
    add_definitions(-DNDEBUG)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if(BUILD_PLATFORM STREQUAL "windows_mingw")
    target_link_libraries(${PROJECT_NAME} -lopengl32 -lgdi32 -lstdc++fs -static -static-libgcc -static-libstdc++)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_WIN32)
elseif(BUILD_PLATFORM STREQUAL "windows_msvc")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_WIN32)
elseif(BUILD_PLATFORM STREQUAL "linux")
    if(ENABLE_ASSIMP)
        add_compile_definitions(GRAVY_ENABLE_ASSIMP)
        find_package(assimp REQUIRED)
        target_link_libraries(${PROJECT_NAME} assimp -pthread -ldl -lm -lstdc++fs)
    elseif()
        target_link_libraries(${PROJECT_NAME} -pthread -ldl -lm -lstdc++fs)
    endif()
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_X11)
elseif(BUILD_PLATFORM STREQUAL "mac")
    target_link_libraries(${PROJECT_NAME} -lstdc++fs -lopengl)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_COCOA)
endif()

if(BUILD_PLATFORM STREQUAL "linux")
    install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION /usr/local/include/GravyEngine3)
endif()