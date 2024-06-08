#include "GameManager.hpp"
#include "Resources.hpp"

static Light *testLight = nullptr;

void GameManager::OnInitialize()
{
    //Light::GetMain()->GetGameObject()->SetIsActive(false);

    Light::GetMain()->GetTransform()->SetRotation(Quaternionf::Euler(45 * Mathf::Deg2Rad, 0, 0));

    Camera *camera = Camera::GetMain();
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();
    camera->GetGameObject()->AddComponent<AudioListener>();

    for(size_t i = 0; i < 3; i++)
    {
        auto lightObject = std::make_shared<GameObject>();
        lightObject->GetTransform()->SetPosition(Vector3(i * 15, 3, i * 15));
        auto light = lightObject->AddComponent<Light>();
        light->SetType(LightType::Point);
        light->SetColor(Color::Red());
        light->SetStrength(1);

        LightObject obj;
        obj.gameObject = lightObject;
        obj.light = light;
        lights.push_back(obj);
    }

    skybox = GameObject::CreatePrimitive(PrimitiveType::Skybox);
    
    ground = GameObject::CreatePrimitive(PrimitiveType::Plane);
    ground->GetTransform()->SetScale(Vector3(1000, 1, 1000));
    auto renderer = ground->GetComponent<MeshRenderer>();
    auto material = renderer->GetMaterial<DiffuseMaterial>(0);
    //material->SetDiffuseColor(Color(42, 107, 14, 255));
    material->SetUVScale(Vector2(200, 200));

    size_t size = 0;
    auto pData = Resources::GetData("Textures/Terrain/forrest_ground_01_diff_1k.jpg", size);

    if(pData && size > 0)
    {
        Image image(pData, size);
        if(image.IsLoaded())
        {
            Texture2D texture(&image);
            auto tex = Texture2D::Add("ForrestGround", texture);

            if(tex != nullptr)
            {
                material->SetDiffuseTexture(tex);
            }
        }
    }

    cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
    cube->GetTransform()->SetPosition(Vector3(0, 0.5f, 0));

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
        float z = Mathf::Sin(Time::GetTimeAsDouble() * 1.0f) * 100;
        cube->GetTransform()->SetPosition(Vector3(x, 2, z));
        cube->GetTransform()->Rotate(Vector3(1, 0, 1));
    }

    for(size_t i = 0; i < lights.size(); i++)
    {
        float x = Mathf::Cos((Time::GetTimeAsDouble() + i * 360) * 1.0f) * 50;
        float z = Mathf::Sin((Time::GetTimeAsDouble() + i * 360) * 1.0f) * 50;
        lights[i].gameObject->GetTransform()->SetPosition(Vector3(x, 2, z));
    }
}

void GameManager::OnGUI()
{
    ImGui::Begin("Light Settings");

    for(size_t i = 0; i < lights.size(); i++)
    {
        auto light = lights[i].light;
        auto obj = lights[i].gameObject.get();

        auto color = light->GetColor();
        auto strength = light->GetStrength();
        auto position = obj->GetTransform()->GetPosition();

        std::string labelColor = "Color Light " + std::to_string(i+1);
        std::string labelStrength = "Strength Light " + std::to_string(i+1);
        std::string labelPosition = "Position Light " + std::to_string(i+1);

        if(ImGui::ColorEdit3(labelColor.c_str(), &color.r))
            light->SetColor(color);
        if(ImGui::DragFloat(labelStrength.c_str(), &strength))
            light->SetStrength(strength);
        if(ImGui::DragFloat3(labelPosition.c_str(), &position.x))
            obj->GetTransform()->SetPosition(position);
    }

    ImGui::End();
}

void GameManager::OnApplicationQuit()
{
    Texture2D::Remove("ForrestGround");
}