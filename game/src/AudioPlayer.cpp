#include "AudioPlayer.hpp"
#include "ResourceManager.hpp"

void AudioPlayer::OnInitialize()
{
    auto &audioClips = ResourceManager::GetAudioClips();

    clipIndex = 0;
    audioSource = GetGameObject()->AddComponent<AudioSource>();
    audioSource->end += [this] (AudioSource *source) { OnAudioEnded(source); };

    audioSource->Play(&audioClips[0]);
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
    auto &audioClips = ResourceManager::GetAudioClips();

    clipIndex++;
    if(clipIndex >= audioClips.size())
        clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}

void AudioPlayer::PreviousTrack()
{
    auto &audioClips = ResourceManager::GetAudioClips();

    audioSource->Stop();
    audioSource->SetCursor(0);

    clipIndex--;
    if(clipIndex < 0)
        clipIndex = audioClips.size() - 1;
    audioSource->Play(&audioClips[clipIndex]);
}

void AudioPlayer::NextTrack()
{
    auto &audioClips = ResourceManager::GetAudioClips();

    audioSource->Stop();
    audioSource->SetCursor(0);

    clipIndex++;
    if(clipIndex >= audioClips.size())
        clipIndex = 0;
    audioSource->Play(&audioClips[clipIndex]);
}