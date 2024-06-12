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
    GameObject *skybox;
    GameObject *ground;
    GameObject *cube;
    AudioSource *audioSourceCube;
};

#endif