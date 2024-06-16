#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "../../include/GravyEngine.hpp"
#include <vector>
#include <memory>

class LightObject
{
public:
    GameObject *gameObject;
    Light *light;
};

class GameManager : public GameBehaviour
{
protected:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnGUI() override;
private:
    GameObject *skybox;
    GameObject *ground;
    GameObject *cube;
    GameObject *particles;
    ParticleSystem *particleSystem;
    std::vector<LightObject> lights;
    AudioSource *audioSourceCube;
    Texture2D *textureGround;
    Texture2D *textureLogo;
    void FindTextures();
    void SetupCamera();
    void SetupLights();
    void SetupModels();
    void SetupAudio();
};

#endif