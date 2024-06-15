#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include "../../include/GravyEngine.hpp"

class AudioPlayer : public GameBehaviour
{
protected:
    void OnInitialize() override;
    void OnGUI() override;
    void OnAssetLoadedAsync(uint64_t id, const std::string &name, const std::vector<uint8_t> &data) override;
private:
    AudioSource *audioSource;
    std::vector<AudioClip> audioClips;
    int clipIndex;
    void OnAudioEnded(AudioSource *source);
    void PreviousTrack();
    void NextTrack();
};

#endif