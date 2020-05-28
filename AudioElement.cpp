#include "AudioElement.h"

AudioElement::AudioElement(std::string file)
{
	if (!buffer.loadFromFile(file))
	{
		std::cout << "FAILED TO LOAD: " << file << std::endl;
		//todo: error handling
	}
	sound.setBuffer(buffer);
}
