#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include "../../include/GravyEngine.hpp"

class AudioPlayer : public GameBehaviour
{
protected:
    void OnInitialize() override;
    void OnGUI() override;
private:
    AudioSource *audioSource;
    int clipIndex;
    void OnAudioEnded(AudioSource *source);
    void PreviousTrack();
    void NextTrack();
};

#endif