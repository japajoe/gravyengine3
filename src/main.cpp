#include "GravyEngine.hpp"
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

class GameManager : public GameBehaviour
{
protected:
    void OnGUI() override
    {
        ImGui::Begin("Hello world");
        ImGui::Text("This works");
        ImGui::End();
    }
};

void OnApplicationLoaded()
{
    gameObject = std::make_unique<GameObject>();

    gameObject->AddComponent<GameManager>();
    
    AudioSource *source = gameObject->AddComponent<AudioSource>();
    source->SetIsLooping(true);
    
    AudioClip clip("../res/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3");

    source->Play(&clip);
}