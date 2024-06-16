#include "GameManager.hpp"

static Light *testLight = nullptr;
static bool textureLoaded = false;

void GameManager::OnInitialize()
{
    FindTextures();
    SetupCamera();
    SetupLights();
    SetupModels();
    SetupAudio();
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
        float z = Mathf::Sin(Time::GetTimeAsDouble() * 1.0f) * 50;
        cube->GetTransform()->SetPosition(Vector3(x, 2, z));
        cube->GetTransform()->Rotate(Vector3(1, 0, 1));
    }

    for(size_t i = 0; i < lights.size(); i++)
    {
        float x = Mathf::Cos((Time::GetTimeAsDouble() * 1.0f) + i * 360) * 50;
        float z = Mathf::Sin((Time::GetTimeAsDouble() * 1.0f) + i * 360) * 50;
        lights[i].gameObject->GetTransform()->SetPosition(Vector3(x, 2, z));
    }

    if(particleSystem)
    {
        particleSystem->Emit(1);
    }
}

void GameManager::OnGUI()
{
    Vector2 screenSize = Screen::GetSize();
    float width = textureLogo->GetWidth() * 0.25f;
    float height = textureLogo->GetHeight() * 0.25f;
    float x = screenSize.x - width - 10;
    float y = screenSize.y - height - 10;

    float alpha = (Mathf::Sin(Time::GetTimeAsDouble() * 0.5f) + 1.0f) * 0.5f;
    alpha = 0.2f + (0.8f * alpha);
    ImVec4 color(1.0f, 1.0f, 1.0f, alpha);

    ImVec4 rect(x, y, width, height);
    ImGuiEx::BeginHideWindow("Logo", rect);
    ImGuiEx::Image(textureLogo->GetId(), ImVec2(width, height), ImVec2(0, 0), ImVec2(1, 1), color);
    ImGuiEx::EndHideWindow();
}

void GameManager::FindTextures()
{
    textureGround = Texture2D::Find("Textures/Terrain/forrest_ground_01_diff_1k.jpg");
    textureLogo = Texture2D::Find("../res/textures/GravyLogoTransparent.png");
}

void GameManager::SetupCamera()
{
    Camera *camera = Camera::GetMain();
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();
    camera->GetGameObject()->AddComponent<AudioListener>();
}

void GameManager::SetupLights()
{
    //Light::GetMain()->GetGameObject()->SetIsActive(false);
    Light::GetMain()->SetStrength(0.1f);
    Light::GetMain()->GetTransform()->SetRotation(Quaternionf::Euler(45 * Mathf::Deg2Rad, 0, 0));

    Color lightColors[3] = {
        Color::Red(),
        Color::Green(),
        Color::Blue()
    };

    for(size_t i = 0; i < 3; i++)
    {
        auto lightObject = GameObject::CreatePrimitive(PrimitiveType::Sphere);
        lightObject->GetTransform()->SetPosition(Vector3(i * 15, 3, i * 15));
        auto light = lightObject->AddComponent<Light>();
        light->SetType(LightType::Point);
        light->SetColor(lightColors[i]);
        light->SetStrength(1);

        // auto audioSource = lightObject->AddComponent<AudioSource>();
        // audioSource->SetAttenuationModel(AttenuationModel::Exponential);
        // audioSource->SetDopplerFactor(0.1f);
        // audioSource->SetMinDistance(5.0f);
        // audioSource->SetMaxDistance(1000.0f);
        // audioSource->SetSpatial(true);
        // auto generator = audioSource->AddGenerator<FMGenerator>(WaveType::Sine, 110, 1.0f);
        // generator->AddOperator(WaveType::Sine, 55, 1.0f);
        // generator->AddOperator(WaveType::Sine, 22, 0.5f);
        // audioSource->Play();

        LightObject obj;
        obj.gameObject = lightObject;
        obj.light = light;
        lights.push_back(obj);
    }
}

void GameManager::SetupModels()
{
    ground = GameObject::CreatePrimitive(PrimitiveType::Plane);
    ground->GetTransform()->SetScale(Vector3(1000, 1, 1000));
    auto renderer = ground->GetComponent<MeshRenderer>();
    auto material = renderer->GetMaterial<DiffuseMaterial>(0);
    material->SetUVScale(Vector2(200, 200));
    material->SetDiffuseTexture(textureGround);

    cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
    cube->GetTransform()->SetPosition(Vector3(0, 0.5f, 0));
    renderer = cube->GetComponent<MeshRenderer>();
    material = renderer->GetMaterial<DiffuseMaterial>(0);
    material->SetDiffuseColor(Color(237, 160, 5, 255));

    particles = GameObject::CreatePrimitive(PrimitiveType::ParticleSystem);
    particleSystem = particles->GetComponent<ParticleSystem>();
    particleSystem->SetParticleType(ParticleType::Sphere);
    
    skybox = GameObject::CreatePrimitive(PrimitiveType::Skybox);
}

void GameManager::SetupAudio()
{
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