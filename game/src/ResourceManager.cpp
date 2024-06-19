#include "ResourceManager.hpp"
#include "GameManager.hpp"
#include "AudioPlayer.hpp"

std::vector<AudioClip> ResourceManager::audioClips;

#define RESOURCE_COUNT 32

void ResourceManager::OnInitialize()
{
    resourceCount = 0;

    AssetManager::LoadResourcePack("assets", "../res/assets.dat", "assets.dat");
    
    LoadAssetFromResourceAsync( AssetType::Texture, "assets", "Textures/Terrain/forrest_ground_01_diff_1k.jpg");
    LoadAssetFromResourceAsync( AssetType::Texture, "assets", "Textures/Particles/smoke_03.png");
    LoadAssetFromResourceAsync( AssetType::Texture, "assets", "Textures/Misc/GravyLogoTransparent.png");
    LoadAssetFromResourceAsync( AssetType::Texture, "assets", "Textures/Misc/Knob70_89.png");
    LoadAssetFromResourceAsync( AssetType::Texture, "assets", "Textures/Misc/Knob71_101.png");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_240201-01_130_Ebm.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/drumpoekel.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB24082019.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB25122020.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB07042020.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB21022020.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB21042021.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB17042021.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB12092019.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB03072020.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_220806-01_120_Gm_Master_240209.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB07102019.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB16042021_vocals.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB16042021.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_230422-02_v1.01.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/PropellerEngine.wav");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB30082017.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_240131-01_124_Dm.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB19042021.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_230422-02_-_v1.01_-_Drake_-_Jimmy_Cooks.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB23042020.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/Kick.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/WB30082019.mp3");
    LoadAssetFromResourceAsync( AssetType::Audio,   "assets", "Audio/law_-_220207-01.mp3");
}

void ResourceManager::OnGUI()
{
    if(resourceCount == RESOURCE_COUNT)
        return;

    ImGui::Begin("Loading Screen");
    ImGui::Text("Loading resources...");
    ImGui::End();
}

void ResourceManager::OnApplicationQuit()
{
    Texture2D::Remove("Textures/Terrain/forrest_ground_01_diff_1k.jpg");
    Texture2D::Remove("Textures/Particles/smoke_03.png");
    Texture2D::Remove("Textures/Misc/GravyLogoTransparent.png");
    Texture2D::Remove("Textures/Misc/Knob70_89.png");
    Texture2D::Remove("Textures/Misc/Knob71_101.png");
}

void ResourceManager::OnAssetLoadedAsync(const AssetInfo &assetInfo)
{
    if(assetInfo.type == AssetType::Texture)
        LoadTexture2D(assetInfo.name, assetInfo.data);
    else if(assetInfo.type == AssetType::Audio)
        LoadAudioClip(assetInfo.name, assetInfo.data);

    resourceCount++;

    if(resourceCount == RESOURCE_COUNT)
    {
        GetGameObject()->AddComponent<GameManager>();
        GetGameObject()->AddComponent<AudioPlayer>();
    }
}

std::vector<AudioClip> &ResourceManager::GetAudioClips()
{
    return audioClips;
}

void ResourceManager::LoadTexture2D(const std::string &name, const std::vector<uint8_t> &data)
{
    if(data.size() == 0)
        return;
    Image image(data.data(), data.size());
    if(image.IsLoaded())
    {
        Texture2D texture(&image);
        Texture2D::Add(name, texture);
    }
}

void ResourceManager::LoadAudioClip(const std::string &name, const std::vector<uint8_t> &data)
{
    if(data.size() == 0)
        return;
    audioClips.push_back(AudioClip(data));
}