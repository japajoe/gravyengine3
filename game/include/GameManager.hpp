#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "../../include/GravyEngine.hpp"
#include <vector>
#include <memory>

class LightObject
{
public:
    std::shared_ptr<GameObject> gameObject;
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
    std::shared_ptr<GameObject> skybox;
    std::shared_ptr<GameObject> ground;
    std::vector<LightObject> lights;
    std::shared_ptr<GameObject> cube;
    AudioSource *audioSourceCube;
};

#endif