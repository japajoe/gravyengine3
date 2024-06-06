#include "GameManager.hpp"

void GameManager::OnInitialize()
{
    Camera *camera = Camera::GetMain();
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();
    camera->GetGameObject()->AddComponent<AudioListener>();

    light = std::make_shared<GameObject>();
    light->GetTransform()->LookAt(Vector3(100, 1, 100), Vector3f::UnitY());
    light->GetTransform()->SetPosition(Vector3(0, 100, 0));
    auto l = light->AddComponent<Light>();
    l->SetType(LightType::Point);

    skybox = GameObject::CreatePrimitive(PrimitiveType::Skybox);
    
    ground = GameObject::CreatePrimitive(PrimitiveType::Plane);
    ground->GetTransform()->SetScale(Vector3(1000, 1, 1000));
    auto renderer = ground->GetComponent<MeshRenderer>();
    auto material = renderer->GetMaterial<DiffuseMaterial>(0);
    material->SetDiffuseColor(Color(42, 107, 14, 255));

    cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
    cube->GetTransform()->SetPosition(Vector3(0, 1, 0));

    renderer = cube->GetComponent<MeshRenderer>();
    material = renderer->GetMaterial<DiffuseMaterial>(0);
    material->SetDiffuseColor(Color(237, 160, 5, 255));

    audioSourceCube = cube->AddComponent<AudioSource>();
    audioSourceCube->SetAttenuationModel(AttenuationModel::Exponential);
    audioSourceCube->SetDopplerFactor(0.1f);
    audioSourceCube->SetMinDistance(5.0f);
    audioSourceCube->SetMaxDistance(1000.0f);
    audioSourceCube->SetSpatial(true);

    auto noiseGenerator = audioSourceCube->AddGenerator<NoiseGenerator>();
    noiseGenerator->SetFrequency(0.6f);
    audioSourceCube->Play();
}

void GameManager::OnUpdate()
{
    if(Input::GetKeyDown(KeyCode::C))
    {
        Input::ToggleMouseCursor();
    }

    if(cube)
    {
        float x = 0;
        float z = Mathf::Sin(Time::GetTimeAsDouble() * 1) * 100;
        cube->GetTransform()->SetPosition(Vector3(x, 1, z));
    }
}

void GameManager::OnGUI()
{

}