#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "../../include/GravyEngine.hpp"
#include <string>
#include <vector>
#include <cstdint>

class ResourceManager : public GameBehaviour
{
public:
    static std::vector<AudioClip> &GetAudioClips();
protected:
    void OnInitialize() override;
    void OnGUI() override;
    void OnApplicationQuit() override;
    void OnAssetLoadedAsync(uint64_t id, const std::string &name, const std::vector<uint8_t> &data) override;
private:
    int resourceCount;
    static std::vector<AudioClip> audioClips;
    void LoadTexture2D(const std::string &name, const std::vector<uint8_t> &data);
    void LoadAudioClip(const std::string &name, const std::vector<uint8_t> &data);
};

#endif