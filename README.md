# GravyEngine
A general purpose component based game engine.

# Features
- UI rendering.
- Terrain renderer.
- Particle system.
- Procedural skybox.
- Primitive types such as cube/sphere/plane etc.
- Directional and point lighting.
- Cascaded shadow mapping.
- Advanced audio engine with 3D spatialization.
- Object picking with raycasts.
- Asset packing.
- Asynchronous asset loading.

# About
The idea is to have a game engine that is easy to use if you are familiar with Unity. The basic workflow is to create a game object and attach components to it which implement some type of behaviour. For example Unity has `MonoBehaviour` as a base class for scripts, while GravyEngine uses `GameBehaviour`. Game objects are created with the `GameObject::Create` method and components are added with the `AddComponent<T>` method. To retrieve a component from a GameObject you use the `GetComponent<T>`method. Keep in mind that the minimum required OpenGL version is 4.5.

# Important notes
- Only use the `GameObject::Create` method to create game objects. This is necessary so the object is properly registered by the engine.
- Likewise, only use `AddComponent` to add components to a game object.
- To destroy a game object, use `GameObject::Destroy`.
- Don't call `GetTransform` or `GetGameObject` in a constructor of a `Component` because it will return NULL. Any initialization that needs to use the transform or game object pointer needs to do this in the `OnInitialize` method.

# Building
To compile the library you need CMake. Most of the required libraries (GLFW/imgui/miniaudioex/glm/glad) are already included. If you are on Linux you'll need to have `assimp` and `bullet` as well. The engine is developed on Linux and can also be cross compiled for windows with mingw32-64. I've also tested to compile with MSVC in a Windows 10 virtual machine and it worked without problems. If you are on MacOS then I can unfortunately not help you, thank Apple for being hostile towards developers and also not supporting latest OpenGL versions.

This command works for building on Linux with gcc/g++.
```bash
cmake -DBUILD_PLATFORM=linux ..
cmake --build .
```

For cross compiling (on Linux) you might have to coerce CMake a little so it uses the right compiler.
```bash
cmake -DBUILD_PLATFORM=windows_mingw -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix ..-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix
cmake --build .
```

If you are on Windows and use MSVC then this command should work.
```bash
cmake -DBUILD_PLATFORM=windows_msvc ..
cmake --build .
```

# Demo application
```cpp
#include <GravyEngine3/GravyEngine.hpp>

static void OnApplicationLoaded();

int main()
{
    Application application("Gravy Engine Demo", 512, 512);
    application.loaded = OnApplicationLoaded;
    application.Run();    
    return 0;
}

class Demo : public GameBehaviour
{
protected:
    void OnInitialize() override
    {
        auto camera = Camera::GetMain();
        camera->AddComponent<FirstPersonCamera>();

        cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
        skybox = GameObject::CreatePrimitive(PrimitiveType::Skybox);
    }

    void OnUpdate() override
    {
        float x = Mathf::Cos(Time::GetTime() * 1.0f) * 50;
        float z = Mathf::Sin(Time::GetTime() * 1.0f) * 50;
        cube->GetTransform()->SetPosition(Vector3(x, 0, z));
    }

    void OnGUI() override
    {
        ImGui::Begin("Demo");
        ImGui::Text("Hello world");
        ImGui::End();
    }
private:
    GameObject *cube;
    GameObject *skybox;
};

void OnApplicationLoaded()
{    
    auto gameObject = GameObject::Create();
    gameObject->AddComponent<Demo>();
}
```