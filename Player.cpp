#include <SFML/Graphics.hpp>
#include "Player.h"

void Player::HandleNewMovementSquare(Map& map)
{
	//check for collectables
	Collectable::TryGetCollectableAtMapPos(*this, mapLocation);

	Character::HandleNewMovementSquare(map); //use base class new square handling
}

void Player::HandleAnimation()
{
	if (animationClock.getElapsedTime().asMilliseconds() > 100 &&
		(currentMoveDir != Direction::None || currentMoveDir == Direction::None && currentAnimationFrame != 0))
	{

		animationClock.restart();
		currentAnimationFrame++;

		if (currentAnimationFrame == numOfAnimationFrames)
		{
			currentAnimationFrame = 0;
		}

		rectSource.left = currentAnimationFrame * 100;
		sprite.setTextureRect(rectSource);
	}

	//movement/mouth audio
	if (currentMoveDir == Direction::None && 
		currentAnimationFrame == 0 && 
		AudioManager::sounds["waka"]->sound.getStatus() == sf::SoundSource::Status::Playing)
		AudioManager::sounds["waka"]->sound.stop();
	else if (currentMoveDir != Direction::None &&
		AudioManager::sounds["waka"]->sound.getStatus() != sf::SoundSource::Status::Playing)
		AudioManager::sounds["waka"]->sound.play();
}
