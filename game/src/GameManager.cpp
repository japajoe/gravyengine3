#include "GameManager.hpp"

void GameManager::OnInitialize()
{
    Camera *camera = Camera::GetMain();
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();

    skybox = GameObject::CreatePrimitive(PrimitiveType::Skybox);
    
    {
        ground = GameObject::CreatePrimitive(PrimitiveType::Plane);
        ground->GetTransform()->SetScale(Vector3(1000, 1, 1000));
        auto renderer = ground->GetComponent<MeshRenderer>();
        auto material = renderer->GetMaterial<DiffuseMaterial>(0);
        material->SetDiffuseColor(Color(42, 107, 14, 255));
    }

    for(size_t i = 0; i < 3; i++)
    {
        auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
        cube->GetTransform()->SetPosition(Vector3((i * 2), 1, -3));

        auto renderer = cube->GetComponent<MeshRenderer>();
        auto material = renderer->GetMaterial<DiffuseMaterial>(0);
        material->SetDiffuseColor(Color(237, 160, 5, 255));

        cubes.push_back(cube);
    }

    std::vector<std::string> audioFiles = {
        "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3",
        "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3",
        "Audio/law_-_090023.mp3",
        "Audio/law_-_220207-01.mp3",
        "Audio/law_-_220806-01_120_Gm_Master_240209.mp3",
        "Audio/law_-_230422-02_v1.01.mp3",
        "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3",
        "Audio/law_-_240131-01_124_Dm.mp3",
        "Audio/law_-_240201-01_130_Ebm.mp3"
    };

    ResourcePack pack;
    pack.LoadPack("../res/assets.dat", "assets.dat");

    if(pack.Loaded())
    {
        for(size_t i = 0; i < audioFiles.size(); i++)
        {
            audioClips.push_back(AudioClip(pack.GetFileData(audioFiles[i])));
        }


        audioSource = GetGameObject()->AddComponent<AudioSource>();
        audioSource->end += [this] (AudioSource *source) { OnAudioEnded(source); };
        
        clipIndex = 0;
        audioSource->Play(&audioClips[clipIndex]);
    }
}

void GameManager::OnUpdate()
{
    if(Input::GetKeyDown(KeyCode::C))
    {
        Input::ToggleMouseCursor();
    }
}

void GameManager::OnGUI()
{
    ImGui::Begin("Hello world");
    ImGui::Text("This works");
    ImGui::End();
}

void GameManager::OnAudioEnded(AudioSource *source)
{
    clipIndex++;
    if(clipIndex >= audioClips.size())
        clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}