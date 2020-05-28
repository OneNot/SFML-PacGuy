#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include "AudioElement.h"

class AudioManager
{
public:
	static std::map<std::string, std::unique_ptr<AudioElement>> sounds;
	static void InitializeAudio();
};

