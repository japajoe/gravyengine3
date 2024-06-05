#include "GameManager.hpp"
#include <memory>

static void OnApplicationLoaded();
static std::unique_ptr<GameObject> gameObject;

int main()
{
    Application application("Gravy Engine!", 512, 512);
    application.loaded = OnApplicationLoaded;
    application.Run();    
    return 0;
}

void OnApplicationLoaded()
{
    gameObject = std::make_unique<GameObject>();
    gameObject->AddComponent<GameManager>();
}