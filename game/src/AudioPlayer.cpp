#include "AudioPlayer.hpp"

void AudioPlayer::OnInitialize()
{
    std::vector<std::string> audioFiles = {
        "Audio/Beats_by_Law_-_230422-02_160_Fm_Automatic.mp3",
        "Audio/Beats_by_Law_-_240217-01_96_Gm_Tangeray.mp3",
        "Audio/law_-_090023.mp3",
        "Audio/law_-_220207-01.mp3",
        "Audio/law_-_220806-01_120_Gm_Master_240209.mp3",
        "Audio/law_-_230422-02_v1.01.mp3",
        "Audio/law_-_230808-02_-_Bandmanrill_-_Copy_and_Paste.mp3",
        "Audio/law_-_240131-01_124_Dm.mp3",
        "Audio/law_-_240201-01_130_Ebm.mp3"
    };

    ResourcePack pack;
    pack.LoadPack("../res/assets.dat", "assets.dat");

    if(pack.Loaded())
    {
        for(size_t i = 0; i < audioFiles.size(); i++)
        {
            audioClips.push_back(AudioClip(pack.GetFileData(audioFiles[i])));
        }

        audioSource = GetGameObject()->AddComponent<AudioSource>();
        audioSource->end += [this] (AudioSource *source) { OnAudioEnded(source); };
        
        clipIndex = 0;
        audioSource->Play(&audioClips[clipIndex]);
    }
}

void AudioPlayer::OnGUI()
{
    ImGui::Begin("Audio Settings");
    
    float masterVolume = AudioContext::GetMasterVolume();
    float musicVolume = audioSource->GetVolume();
    
    if(ImGuiEx::Knob("Master Volume", &masterVolume, 0.0f, 1.0f, 64))
        AudioContext::SetMasterVolume(masterVolume);
    
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