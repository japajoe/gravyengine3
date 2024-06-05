#include "GameManager.hpp"

void GameManager::OnInitialize()
{
    AudioSource *source = GetGameObject()->AddComponent<AudioSource>();
    source->SetIsLooping(true);
    
    AudioClip clip("../res/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3");

    source->Play(&clip);

    for(size_t i = 0; i < 3; i++)
    {
        auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
        cube->GetTransform()->SetPosition(Vector3((i * 2), 0, -3));
        cubes.push_back(cube);
    }

    Camera *camera = Camera::GetMain();
    camera->SetClearColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();
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