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
    std::vector<std::shared_ptr<GameObject>> cubes;
};

#endif