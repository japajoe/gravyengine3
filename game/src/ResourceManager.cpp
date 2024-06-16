#include "ResourceManager.hpp"
#include "GameManager.hpp"
#include "AudioPlayer.hpp"

std::vector<AudioClip> ResourceManager::audioClips;

void ResourceManager::OnInitialize()
{
    resourceCount = 0;

    AssetManager::LoadResourcePack("assets", "../res/assets.dat", "assets.dat");
    
    LoadAssetFromResourceAsync( 0, "assets", "Textures/Terrain/forrest_ground_01_diff_1k.jpg");
    LoadAssetFromFileAsync(     1, "assets", "../res/textures/GravyLogoTransparent.png");
    LoadAssetFromResourceAsync( 2, "assets", "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3");
    LoadAssetFromResourceAsync( 3, "assets", "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3");
    LoadAssetFromResourceAsync( 4, "assets", "Audio/law_-_090023.mp3");
    LoadAssetFromResourceAsync( 5, "assets", "Audio/law_-_220207-01.mp3");
    LoadAssetFromResourceAsync( 6, "assets", "Audio/law_-_220806-01_120_Gm_Master_240209.mp3");
    LoadAssetFromResourceAsync( 7, "assets", "Audio/law_-_230422-02_v1.01.mp3");
    LoadAssetFromResourceAsync( 8, "assets", "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3");
    LoadAssetFromResourceAsync( 9, "assets", "Audio/law_-_240131-01_124_Dm.mp3");
    LoadAssetFromResourceAsync( 10, "assets", "Audio/law_-_240201-01_130_Ebm.mp3");
}

void ResourceManager::OnGUI()
{
    if(resourceCount == 11)
        return;

    ImGui::Begin("Loading Screen");
    ImGui::Text("Loading resources...");
    ImGui::End();
}

void ResourceManager::OnApplicationQuit()
{
    Texture2D::Remove("Textures/Terrain/forrest_ground_01_diff_1k.jpg");
    Texture2D::Remove("../res/textures/GravyLogoTransparent.png");
}

void ResourceManager::OnAssetLoadedAsync(uint64_t id, const std::string &name, const std::vector<uint8_t> &data)
{
    if(data.size() == 0)
        return;

    if(id == 0)
    {
        LoadTexture2D(name, data);
    }
    else if(id == 1)
    {
        LoadTexture2D(name, data);
    }
    else if(id >= 2)
    {
        LoadAudioClip(name, data);
    }

    if(id == 10)
    {
        GetGameObject()->AddComponent<GameManager>();
        GetGameObject()->AddComponent<AudioPlayer>();
    }

    resourceCount++;
}

std::vector<AudioClip> &ResourceManager::GetAudioClips()
{
    return audioClips;
}

void ResourceManager::LoadTexture2D(const std::string &name, const std::vector<uint8_t> &data)
{
    Image image(data.data(), data.size());
    if(image.IsLoaded())
    {
        Texture2D texture(&image);
        Texture2D::Add(name, texture);
    }
}

void ResourceManager::LoadAudioClip(const std::string &name, const std::vector<uint8_t> &data)
{
    audioClips.push_back(AudioClip(data));
}