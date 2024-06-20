# GravyEngine
A general purpose component based game engine. The idea is to have a game engine that is easy to use if you are familiar with Unity. The basic workflow is to create a game object and attach components to it which implement some type of behaviour. For example Unity has `MonoBehaviour` as a base class for scripts, while GravyEngine uses `GameBehaviour`. Game objects are created with the `GameObject::Create` method and components are added with the `AddComponent<T>` method. To retrieve a component from a GameObject you use the `GetComponent<T>`method. Keep in mind that the minimum required OpenGL version is 4.5.

# Features
- UI rendering.
- Terrain renderer.
- Particle system.
- Procedural skybox.
- Primitive types such as cube/sphere/plane etc.
- Directional and point lighting.
- Cascaded shadow mapping.
- Advanced audio engine with 3D spatialization.
- Resource (asset) packing and loading.

# Building
To compile the library you need CMake. All required libraries (GLFW/imgui/miniaudioex/glm/glad) are already included. The engine is developed on Linux and can also be cross compiled for windows with mingw32-64. I've also tested to compile with MSVC in a Windows 10 virtual machine and it worked without problems. If you are on MacOS then I can unfortunately not help you, thank Apple for being hostile towards developers.

This commands works for building on Linux with gcc/g++.
```bash
cmake -DBUILD_PLATFORM=linux
cmake --build .
```

For cross compiling you might have to coerce CMake a little so it uses the right compiler.
```bash
cmake -DBUILD_PLATFORM=windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix
cmake --build .
```

If you are on Windows and use MSVC then this command should work.
```bash
cmake -DBUILD_PLATFORM=windows
cmake --build .
```