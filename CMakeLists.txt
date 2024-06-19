cmake_minimum_required(VERSION 3.12)
#project(gravyengine3)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(libs/glfw)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c game/*.cpp)
#file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/include/Audio"
    "${PROJECT_SOURCE_DIR}/include/Audio/DSP"
    "${PROJECT_SOURCE_DIR}/include/Core"
    "${PROJECT_SOURCE_DIR}/include/External"
    "${PROJECT_SOURCE_DIR}/include/External/glad"
    "${PROJECT_SOURCE_DIR}/include/External/GLFW"
    "${PROJECT_SOURCE_DIR}/include/External/imgui"
    "${PROJECT_SOURCE_DIR}/include/External/miniaudioex"
    "${PROJECT_SOURCE_DIR}/include/Rendering"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Buffers"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Materials"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Shaders"
    "${PROJECT_SOURCE_DIR}/include/System"
    "${PROJECT_SOURCE_DIR}/include/System/Collections"
    "${PROJECT_SOURCE_DIR}/include/System/Drawing"
    "${PROJECT_SOURCE_DIR}/include/System/IO"
    "${PROJECT_SOURCE_DIR}/include/System/Numerics"
    "${PROJECT_SOURCE_DIR}/game/include"
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(GRAVY_DEBUG_LOGGING)
else()
    add_definitions(-DNDEBUG)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
#add_library(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN/runtimes"  # Relative to the executable
)

if(BUILD_PLATFORM STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} -lopengl32 -lgdi32 -lstdc++fs -static -static-libgcc -static-libstdc++)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_WIN32)
elseif(BUILD_PLATFORM STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} -pthread -ldl -lm -lstdc++fs)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_X11)
elseif(BUILD_PLATFORM STREQUAL "mac")
    target_link_libraries(${PROJECT_NAME} -lstdc++fs -lopengl)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_COCOA)
endif()

#install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib)
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION /usr/local/include/GravyEngine3)