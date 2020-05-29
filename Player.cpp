#include <SFML/Graphics.hpp>
#include "Player.h"

Player* Player::player = nullptr;

void Player::HandleNewMovementSquare(Map& map)
{
	//check for collectables
	Collectable::TryGetCollectableAtMapPos(*this, mapLocation);

	Character::HandleNewMovementSquare(map); //use base class new square handling
}

void Player::CollisionCheck()
{
	//Can't be bothered to make anything fancier. rectangle bounding box collision will do.
	//scaled down a bit so as to be closer to proper collision

	if (GameManager::GetGameState() == GameState::Playing)
	{
		//player bounds
		sf::FloatRect pb = sprite.getGlobalBounds();

		//scaling it down 30% while preserving the same mid point
		pb.left += pb.width * 0.15f;
		pb.top += pb.height * 0.15f;
		pb.height *= 0.7f;
		pb.width *= 0.7f;

		//check each enemy
		for (int i = 0; i < Enemy::enemies.size(); i++)
		{
			//enemy bounds
			sf::FloatRect eb = Enemy::enemies.at(i)->sprite.getGlobalBounds();

			//scaling it down 30% while preserving the same mid point
			eb.left += eb.width * 0.15f;
			eb.top += eb.height * 0.15f;
			eb.height *= 0.7f;
			eb.width *= 0.7f;

			if (pb.intersects(eb))
			{
				std::cout << "PLAYER <-> ENEMY COLLISION!" << std::endl;
				GameManager::LoseGame();
				break;
			}
		}
	}
	
	
}

Player::Player(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition) :
	Character::Character(renderWindow, map, characterTextureToUse, spawnMapPosition)
{
	player = this;
}

void Player::HandleAnimation()
{
	Character::HandleAnimation();

	//movement/mouth audio
	if (GameManager::GetGameState() == GameState::Playing &&
		currentMoveDir == Direction::None && 
		currentAnimationFrame == 0 && 
		AudioManager::sounds["waka"]->sound.getStatus() == sf::SoundSource::Status::Playing)
		AudioManager::sounds["waka"]->sound.stop();
	else if (GameManager::GetGameState() == GameState::Playing && 
		currentMoveDir != Direction::None &&
		AudioManager::sounds["waka"]->sound.getStatus() != sf::SoundSource::Status::Playing)
		AudioManager::sounds["waka"]->sound.play();
}
