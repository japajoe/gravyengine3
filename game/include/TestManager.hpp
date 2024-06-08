#ifndef TESTMANAGER_HPP
#define TESTMANAGER_HPP

#include "../../include/GravyEngine.hpp"
#include <vector>
#include <memory>


class TestManager : public GameBehaviour
{
protected:
    void OnInitialize() override;
    void OnUpdate() override;
private:
    std::shared_ptr<GameObject> skybox;
    std::shared_ptr<GameObject> ground;
    std::shared_ptr<GameObject> cube;
    AudioSource *audioSourceCube;
};

#endif