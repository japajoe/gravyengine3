#include "AudioPlayer.hpp"
#include "Resources.hpp"

void AudioPlayer::OnInitialize()
{
    std::vector<std::string> audioFiles = {
        "../res/audio/law_-_240201-01_130_Ebm.mp3",
        "../res/audio/drumpoekel.mp3",
        "../res/audio/WB24082019.mp3",
        "../res/audio/WB25122020.mp3",
        "../res/audio/WB07042020.mp3",
        "../res/audio/WB21022020.mp3",
        "../res/audio/WB21042021.mp3",
        "../res/audio/WB17042021.mp3",
        "../res/audio/WB12092019.mp3",
        "../res/audio/WB03072020.mp3",
        "../res/audio/law_-_220806-01_120_Gm_Master_240209.mp3",
        "../res/audio/WB07102019.mp3",
        "../res/audio/WB16042021_vocals.mp3",
        "../res/audio/WB16042021.mp3",
        "../res/audio/law_-_230422-02_v1.01.mp3",
        "../res/audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3",
        "../res/audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3",
        "../res/audio/WB30082017.mp3",
        "../res/audio/law_-_240131-01_124_Dm.mp3",
        "../res/audio/WB19042021.mp3",
        "../res/audio/law_-_230422-02_-_v1.01_-_Drake_-_Jimmy_Cooks.mp3",
        "../res/audio/WB23042020.mp3",
        "../res/audio/Kick.mp3",
        "../res/audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3",
        "../res/audio/WB30082019.mp3",
        "../res/audio/law_-_220207-01.mp3"
    };

    // std::vector<std::string> audioFiles = {
    //     "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3",
    //     "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3",
    //     "Audio/law_-_090023.mp3",
    //     "Audio/law_-_220207-01.mp3",
    //     "Audio/law_-_220806-01_120_Gm_Master_240209.mp3",
    //     "Audio/law_-_230422-02_v1.01.mp3",
    //     "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3",
    //     "Audio/law_-_240131-01_124_Dm.mp3",
    //     "Audio/law_-_240201-01_130_Ebm.mp3"
    // };

    // for(size_t i = 0; i < audioFiles.size(); i++)
    // {
    //     size_t size = 0;
    //     auto pData = Resources::GetData(audioFiles[i], size);
    //     if(pData && size > 0)
    //     {
    //         audioClips.push_back(AudioClip(pData, size));
    //     }
    // }

    for(size_t i = 0; i < audioFiles.size(); i++)
    {
        audioClips.push_back(AudioClip(audioFiles[i], true));
    }

    audioSource = GetGameObject()->AddComponent<AudioSource>();
    audioSource->end += [this] (AudioSource *source) { OnAudioEnded(source); };
    
    clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}

void AudioPlayer::OnGUI()
{
    ImGui::Begin("Audio Settings");
    
    float masterVolume = AudioContext::GetMasterVolume();
    float musicVolume = audioSource->GetVolume();
    
    if(ImGuiEx::Knob("Master Volume", &masterVolume, 0.0f, 1.0f, 64))
        AudioContext::SetMasterVolume(masterVolume);

    ImGui::SameLine();
    
    if(ImGuiEx::Knob("Music Volume", &musicVolume, 0.0f, 1.0f, 64))
        audioSource->SetVolume(musicVolume);
    
    if(ImGui::Button("<"))
        PreviousTrack();
    
    ImGui::SameLine();
    
    if(ImGui::Button(">"))
        NextTrack();
    
    ImGui::End();
}

void AudioPlayer::OnAudioEnded(AudioSource *source)
{
    clipIndex++;
    if(clipIndex >= audioClips.size())
        clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}

void AudioPlayer::PreviousTrack()
{
    audioSource->Stop();
    audioSource->SetCursor(0);

    clipIndex--;
    if(clipIndex < 0)
        clipIndex = audioClips.size() - 1;
    audioSource->Play(&audioClips[clipIndex]);
}

void AudioPlayer::NextTrack()
{
    audioSource->Stop();
    audioSource->SetCursor(0);

    clipIndex++;
    if(clipIndex >= audioClips.size())
        clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}