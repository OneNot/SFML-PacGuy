#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
//#include "AudioManager.h"

class AudioElement
{
public:
	sf::SoundBuffer buffer;
	sf::Sound sound;
	AudioElement(std::string file);
};

