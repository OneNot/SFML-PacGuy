#include "AudioManager.h"

std::map<std::string, std::unique_ptr<AudioElement>> AudioManager::sounds = std::map<std::string, std::unique_ptr<AudioElement>>();
sf::Music AudioManager::music;

void AudioManager::InitializeAudio()
{
	sounds["waka"] = std::make_unique<AudioElement>("Assets/Audio/pacman_chomp_edit.wav");
	sounds["waka"]->sound.setLoop(true);
	sounds["eat_fruit"] = std::make_unique<AudioElement>("Assets/Audio/pacman_eatfruit.wav");
	sounds["death"] = std::make_unique<AudioElement>("Assets/Audio/pacman_death.wav");

	//MUSIC
	if (!music.openFromFile("Assets/Audio/Pac-man-theme-remix-By-Arsenic19.wav"))
		std::cout << "failed to laod music file" << std::endl;
	music.setLoop(true);
	music.play();
}
