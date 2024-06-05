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
    std::shared_ptr<GameObject> skybox;
    std::shared_ptr<GameObject> ground;
    AudioSource *audioSource;
    std::vector<AudioClip> audioClips;
    size_t clipIndex;
    void OnAudioEnded(AudioSource *source);
};

#endif