#include "Character.h"

Character::Character(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition)
{
	if (!texture.loadFromFile(characterTextureToUse))
	{
		std::cout << "Failed to load: " << characterTextureToUse << std::endl;
		//window.close(); //TODO: replace with some error handling system
	}
	texture.setSmooth(true);
	sprite = sf::Sprite(texture);
	sprite.setScale(map.scale, map.scale);
	sf::FloatRect pacGuySpriteBounds(sprite.getLocalBounds());

	//Currently just putting it in the middle if no custom value is given
	//if a custom value is given, but it's not a valid position, then also use this
	//we can assume that 0,0 is not a custom value because even if it is, that's not a valid value anyways as it would be in a wall
	if (spawnMapPosition == sf::Vector2i::Vector2(0, 0))
	{
		sprite.setPosition(renderWindow.getSize().x / 2 - (pacGuySpriteBounds.left + pacGuySpriteBounds.width * map.scale / 2),
			renderWindow.getSize().y / 2 - (pacGuySpriteBounds.top + pacGuySpriteBounds.height * map.scale / 2));
	}

	//TODO: support custom values for spawn position

	prevSquareMidPos = sprite.getPosition();
	mapLocation = sf::Vector2i(9, 9); //temp, remember to actually calculate dynamically when making support for custom values
	std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
}

void Character::MovementHandling(Map &map, float delta)
{
	//TODO: A lot of repeated code here... Might refactor this a bit later

	sf::Vector2f currentPos = sprite.getPosition(); //current pos before movement

	//===== UP =====
	if (currentMoveDir == Direction::Up)
	{
		//std::cout << "UP " << std::endl;
		sf::Vector2f movement(0.0f, -delta * moveSpeed); //movement to be made
		sf::Vector2f nextPos = (currentPos + movement); //next pos after movement is done

		//if next pos would be over the next mid-square...
		if (nextPos.y < (prevSquareMidPos.y - map.scaledSquareSize))
		{
			//...then set player to next mid-square
			nextPos = sf::Vector2f(nextPos.x, prevSquareMidPos.y - map.scaledSquareSize);
			sprite.setPosition(nextPos);

			mapLocation.y--;
			std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
			prevSquareMidPos = nextPos; //set prevSquareMidPos to new pos, as we are now at a new mid-square
			movedSinceLastSquareMid = 0.0f; //reset how much we have moved since last mid-square, as we are now at a new mid-square
			currentMoveDir = nextMoveDirInstruction; //follow next move instruction
			return; //nothing else needs to be done here so we can stop the method
		}
		else
			sprite.move(movement); //...else just move player
	}
	//===== DOWN =====
	else if (currentMoveDir == Direction::Down)
	{
		//std::cout << "DOWN " << std::endl;
		sf::Vector2f movement(0.0f, delta * moveSpeed); //movement to be made
		sf::Vector2f nextPos = (currentPos + movement); //next pos after movement is done

		//if next pos would be over the next mid-square...
		if (nextPos.y > (prevSquareMidPos.y + map.scaledSquareSize))
		{
			//...then set player to next mid-square
			nextPos = sf::Vector2f(nextPos.x, prevSquareMidPos.y + map.scaledSquareSize);
			sprite.setPosition(nextPos);

			mapLocation.y++;
			std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
			prevSquareMidPos = nextPos; //set prevSquareMidPos to new pos, as we are now at a new mid-square
			movedSinceLastSquareMid = 0.0f; //reset how much we have moved since last mid-square, as we are now at a new mid-square
			currentMoveDir = nextMoveDirInstruction; //follow next move instruction
			return; //nothing else needs to be done here so we can stop the method
		}
		else
			sprite.move(movement); //...else just move player
	}
	//===== LEFT =====
	else if (currentMoveDir == Direction::Left)
	{
		//std::cout << "LEFT " << std::endl;
		sf::Vector2f movement(-delta * moveSpeed, 0.0f); //movement to be made
		sf::Vector2f nextPos = (currentPos + movement); //next pos after movement is done

		//if next pos would be over the next mid-square...
		if (nextPos.x < (prevSquareMidPos.x - map.scaledSquareSize))
		{
			//...then set player to next mid-square
			nextPos = sf::Vector2f(prevSquareMidPos.x - map.scaledSquareSize, nextPos.y);
			sprite.setPosition(nextPos);

			mapLocation.x--;
			std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
			prevSquareMidPos = nextPos; //set prevSquareMidPos to new pos, as we are now at a new mid-square
			movedSinceLastSquareMid = 0.0f; //reset how much we have moved since last mid-square, as we are now at a new mid-square
			currentMoveDir = nextMoveDirInstruction; //follow next move instruction
			return; //nothing else needs to be done here so we can stop the method
		}
		else
			sprite.move(movement); //...else just move player
	}
	//===== RIGHT =====
	else if (currentMoveDir == Direction::Right)
	{
		//std::cout << "RIGHT " << std::endl;
		sf::Vector2f movement(delta * moveSpeed, 0.0f); //movement to be made
		sf::Vector2f nextPos = (currentPos + movement); //next pos after movement is done

		//if next pos would be over the next mid-square...
		if (nextPos.x > (prevSquareMidPos.x + map.scaledSquareSize))
		{
			//...then set player to next mid-square
			nextPos = sf::Vector2f(prevSquareMidPos.x + map.scaledSquareSize, nextPos.y);
			sprite.setPosition(nextPos);

			mapLocation.x++;
			std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
			prevSquareMidPos = nextPos; //set prevSquareMidPos to new pos, as we are now at a new mid-square
			movedSinceLastSquareMid = 0.0f; //reset how much we have moved since last mid-square, as we are now at a new mid-square
			currentMoveDir = nextMoveDirInstruction; //follow next move instruction
			return; //nothing else needs to be done here so we can stop the method
		}
		else
			sprite.move(movement); //...else just move player
	}
	else //no move dir yet
	{
		currentMoveDir = nextMoveDirInstruction; //set movedir to the next instruction
		return; //nothing else needs to be done here so we can stop the method
	}

	movedSinceLastSquareMid += delta * moveSpeed; //add to how much we have moved since previous mid-square
}
