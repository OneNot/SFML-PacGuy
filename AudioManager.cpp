#include "AudioManager.h"

std::map<std::string, std::unique_ptr<AudioElement>> AudioManager::sounds = std::map<std::string, std::unique_ptr<AudioElement>>();

void AudioManager::InitializeAudio()
{
	sounds["waka"] = std::make_unique<AudioElement>("Assets/Audio/pacman_chomp_edit.wav");
	sounds["waka"]->sound.setLoop(true);
	sounds["eat_fruit"] = std::make_unique<AudioElement>("Assets/Audio/pacman_eatfruit.wav");
}
