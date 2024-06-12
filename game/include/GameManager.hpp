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
    void OnApplicationQuit();
private:
    GameObject *skybox;
    GameObject *ground;
    GameObject *cube;
    std::vector<LightObject> lights;
    AudioSource *audioSourceCube;
};

#endif