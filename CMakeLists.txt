cmake_minimum_required(VERSION 3.12)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glad)
add_subdirectory(libs/glm)
add_subdirectory(libs/glfw)
add_subdirectory(libs/imgui)
add_subdirectory(libs/miniaudioex)

file(GLOB_RECURSE SOURCES src/*.cpp game/src/*.cpp)

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

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN/runtimes"  # Relative to the executable
)

if(BUILD_PLATFORM STREQUAL "windows")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lstdc++fs -static -static-libgcc -static-libstdc++)
    target_link_libraries(${PROJECT_NAME} -Llibs/assimp/win_x86 -lassimp.dll)
elseif(BUILD_PLATFORM STREQUAL "linux")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lstdc++fs)
    target_link_libraries(${PROJECT_NAME} -Llibs/assimp/linux_x86 -lassimp)
elseif(BUILD_PLATFORM STREQUAL "mac")
    target_link_libraries(${PROJECT_NAME} glad glm glfw imgui miniaudioex -lstdc++fs -lopengl)
endif()

