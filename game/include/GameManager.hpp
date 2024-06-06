#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "../../include/GravyEngine.hpp"
#include <vector>
#include <memory>

class GameManager : public GameBehaviour
{
protected:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnGUI() override;
private:
    std::shared_ptr<GameObject> skybox;
    std::shared_ptr<GameObject> ground;
    std::shared_ptr<GameObject> light;
    std::shared_ptr<GameObject> cube;
    AudioSource *audioSourceCube;
};

#endif