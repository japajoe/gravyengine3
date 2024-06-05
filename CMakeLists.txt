cmake_minimum_required(VERSION 3.12)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glad)
add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/imgui)
add_subdirectory(libs/miniaudioex)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_SOURCE_DIR}/include/Audio"
    "${PROJECT_SOURCE_DIR}/include/Audio/DSP"
    "${PROJECT_SOURCE_DIR}/include/Core"
    "${PROJECT_SOURCE_DIR}/include/External"
    "${PROJECT_SOURCE_DIR}/include/External/glad"
    "${PROJECT_SOURCE_DIR}/include/External/glfw"
    "${PROJECT_SOURCE_DIR}/include/External/imgui"
    "${PROJECT_SOURCE_DIR}/include/External/miniaudioex"
    "${PROJECT_SOURCE_DIR}/include/Rendering"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Buffers"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Materials"
    "${PROJECT_SOURCE_DIR}/include/Rendering/Shaders"
    "${PROJECT_SOURCE_DIR}/include/System"
    "${PROJECT_SOURCE_DIR}/include/System/Numerics"
)

add_executable(${PROJECT_NAME} ${SOURCES})

if(BUILD_PLATFORM STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lopengl32 -lgdi32 -static -static-libgcc -static-libstdc++)
elseif(BUILD_PLATFORM STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lGL -ldl -lm -pthread)
elseif(BUILD_PLATFORM STREQUAL "mac")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lopengl)
endif()
