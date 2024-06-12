#include "GameManager.hpp"
#include "TestManager.hpp"
#include "AudioPlayer.hpp"
#include "Resources.hpp"

#include <memory>

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
    Resources::LoadAll();
    
    gameObject = GameObject::Create();
    gameObject->AddComponent<GameManager>();
    gameObject->AddComponent<AudioPlayer>();
    //gameObject->AddComponent<TestManager>();
}