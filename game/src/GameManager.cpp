#include "GameManager.hpp"
#include "Resources.hpp"

static Light *testLight = nullptr;

void GameManager::OnInitialize()
{
    //Light::GetMain()->GetGameObject()->SetIsActive(false);

    Light::GetMain()->SetStrength(0.1f);

    Light::GetMain()->GetTransform()->SetRotation(Quaternionf::Euler(45 * Mathf::Deg2Rad, 0, 0));

    Camera *camera = Camera::GetMain();
    camera->GetGameObject()->AddComponent<FirstPersonCamera>();
    camera->GetGameObject()->AddComponent<AudioListener>();



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

        LightObject obj;
        obj.gameObject = lightObject;
        obj.light = light;
        lights.push_back(obj);
    }


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
}

void GameManager::OnGUI()
{
    return;
    
    ImGui::Begin("Light Settings");

    auto light = lights[0].light;


    auto color = light->GetColor();
    auto ambient = light->GetAmbient();
    auto diffuse = light->GetDiffuse();
    auto specular = light->GetSpecular();
    auto strength = light->GetStrength();
    auto constant = light->GetConstant();
    auto linear = light->GetLinear();
    auto quadratic = light->GetQuadratic();

    if(ImGui::ColorEdit3("Color", &color.r))
        light->SetColor(color);
    if(ImGui::ColorEdit3("Ambient", &ambient.r))
        light->SetAmbient(ambient);
    if(ImGui::ColorEdit3("Diffuse", &diffuse.r))
        light->SetDiffuse(diffuse);
    if(ImGui::ColorEdit3("Specular", &specular.r))
        light->SetSpecular(specular);
    if(ImGui::DragFloat("Strength", &strength))
        light->SetStrength(strength);
    if(ImGui::DragFloat("Constant", &constant))
        light->SetConstant(constant);
    if(ImGui::DragFloat("Linear", &linear))
        light->SetLinear(linear);
    if(ImGui::DragFloat("Quadratic", &quadratic))
        light->SetQuadratic(quadratic);

    ImGui::End();

    ImGui::Begin("Fog Settings");
    
    Color fogColor = WorldSettings::GetFogColor();
    float fogDensity = WorldSettings::GetFogDensity();
    
    if(ImGui::ColorEdit3("Fog Color", &fogColor.r))
        WorldSettings::SetFogColor(fogColor);
    
    if(ImGui::DragFloat("Fog Density", &fogDensity))
        WorldSettings::SetFogDensity(fogDensity);
    
    ImGui::End();
}

void GameManager::OnApplicationQuit()
{
    Texture2D::Remove("ForrestGround");
}