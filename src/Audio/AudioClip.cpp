#include "AudioClip.hpp"

namespace GravyEngine
{
    AudioClip::AudioClip(const std::string &filepath, bool streamFromDisk)
    {
        this->filePath = filepath;
        this->handle = nullptr;
        this->streamFromDisk = streamFromDisk;
    }

    AudioClip::AudioClip(const std::vector<uint8_t> &data)
    {
        this->data = data;
        this->handle = reinterpret_cast<void*>(const_cast<uint8_t*>(this->data.data()));
        this->streamFromDisk = false;
    }

    std::string AudioClip::GetFilePath() const
    {
        return filePath;
    }

    void *AudioClip::GetHandle() const
    {
        return handle;
    }

    size_t AudioClip::GetDataSize() const
    {
        return data.size();
    }

    bool AudioClip::GetStreamFromDisk() const
    {
        return streamFromDisk;
    }
};