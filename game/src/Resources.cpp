#include "Resources.hpp"

std::unique_ptr<ResourcePack> Resources::pack;
std::unordered_map<std::string,std::vector<uint8_t>> Resources::data;

void Resources::LoadAll()
{
    pack = std::make_unique<ResourcePack>();

    std::vector<std::string> files = {
        // "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3",
        // "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3",
        // "Audio/law_-_090023.mp3",
        // "Audio/law_-_220207-01.mp3",
        // "Audio/law_-_220806-01_120_Gm_Master_240209.mp3",
        // "Audio/law_-_230422-02_v1.01.mp3",
        // "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3",
        // "Audio/law_-_240131-01_124_Dm.mp3",
        // "Audio/law_-_240201-01_130_Ebm.mp3",
        "Textures/Terrain/forrest_ground_01_diff_1k.jpg"
    };

    if(pack->LoadPack("../res/assets.dat", "assets.dat"))
    {
        for(size_t i = 0; i < files.size(); i++)
        {
            auto buffer = pack->GetFileData(files[i]);

            if(buffer.size() > 0)
            {
                data[files[i]] = buffer;
            }
        }
    }
}

uint8_t *Resources::GetData(const std::string &filename, size_t &size)
{
    if(data.count(filename) > 0)
    {
        auto pData = &data[filename];
        size = pData->size();

        if(size == 0)
            return nullptr;

        return pData->data();
    }
    
    size = 0;
    
    return nullptr;
}