#include "TestManager.hpp"
#include "ResourceManager.hpp"

static void OnApplicationLoaded();
static GameObject *gameObject;

int main()
{
    Application application("Gravy Engine", 512, 512);
    application.loaded = OnApplicationLoaded;
    application.Run();    
    return 0;
}

void OnApplicationLoaded()
{    
    gameObject = GameObject::Create();
    gameObject->AddComponent<ResourceManager>();
    //gameObject->AddComponent<TestManager>();
}