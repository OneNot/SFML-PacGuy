#include "Character.h"

Character::Character(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition)
{
	if (!texture.loadFromFile(characterTextureToUse))
	{
		std::cout << "Failed to load: " << characterTextureToUse << std::endl;
		//window.close(); //TODO: replace with some error handling system
	}
	texture.setSmooth(true);
	rectSource = sf::IntRect(0, 0, 100, 100);
	sprite = sf::Sprite(texture, rectSource);
	currentAnimationFrame = 0;
	rotationPosOffset = sf::Vector2f(0, 0);
	sprite.setScale(map.scale, map.scale);
	sf::FloatRect pacGuySpriteBounds(sprite.getLocalBounds());

	//Currently just putting it in mainSpawnLocation if no custom value is given
	//if a custom value is given, but it's not a valid position, then also use this
	//we can assume that 0,0 is not a custom value because even if it is, that's not a valid value anyways as it would be in a wall
	if (spawnMapPosition == sf::Vector2i::Vector2(0, 0))
	{
		sprite.setPosition(map.sprite.getPosition() + sf::Vector2f((map.scaledSquareSize * map.mainSpawnMapLocation.x), (map.scaledSquareSize * map.mainSpawnMapLocation.y)));
		mapLocation = map.mainSpawnMapLocation;
	}
	else
	{
		sprite.setPosition(map.sprite.getPosition() + sf::Vector2f((map.scaledSquareSize * spawnMapPosition.x), (map.scaledSquareSize * spawnMapPosition.y)));
		mapLocation = spawnMapPosition;
	}

	prevSquareMidPos = sprite.getPosition();
	std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;

	animationClock = sf::Clock();
}

void Character::MovementHandling(Map &map, float delta)
{
	//TODO: A lot of repeated code here... Might refactor this a bit later

	CollisionCheck();
	HandleAnimation();

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

			mapLocation.y--; //set new map data position
			HandleNewMovementSquare(map); //do the operations needed when reaching a new square (map data location)
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

			mapLocation.y++; //set new map data position
			HandleNewMovementSquare(map); //do the operations needed when reaching a new square (map data location)
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

			mapLocation.x--; //set new map data position
			HandleNewMovementSquare(map); //do the operations needed when reaching a new square (map data location)
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

			mapLocation.x++; //set new map data position
			HandleNewMovementSquare(map); //do the operations needed when reaching a new square (map data location)
		}
		else
			sprite.move(movement); //...else just move player
	}
	else //no move dir yet
		HandleNewMovementSquare(map); //do the operations needed when reaching a new square (map data location)
}

void Character::HandleNewMovementSquare(Map& map)
{
	if (mapLocation == map.teleports[0])
	{
		//teleport to 1
		sprite.setPosition(map.MapPosToRealPos(map.teleports[1]) + rotationPosOffset);
		mapLocation = map.teleports[1];
	}
	else if (mapLocation == map.teleports[1])
	{
		//teleport to 0
		sprite.setPosition(map.MapPosToRealPos(map.teleports[0]) + rotationPosOffset);
		mapLocation = map.teleports[0];
	}
	else
	{
		//===== next instruction is UP and is valid =====//
		if (nextMoveDirInstruction == Direction::Up && map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y - 1)) == 0)
		{
			currentMoveDir = nextMoveDirInstruction; //set the next move instruction as the current followed instruction
			sprite.setRotation(-90);
			sprite.move(-rotationPosOffset); //remove the current rotation offset from position
			rotationPosOffset = sf::Vector2f(0.0f, 100.0f * map.scale); //set new offset
			sprite.move(rotationPosOffset); //apply new offset
		}

		//===== next instruction is DOWN and is valid =====//
		else if (nextMoveDirInstruction == Direction::Down && map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y + 1)) == 0)
		{
			currentMoveDir = nextMoveDirInstruction; //set the next move instruction as the current followed instruction
			sprite.setRotation(90);
			sprite.move(-rotationPosOffset); //remove the current rotation offset from position
			rotationPosOffset = sf::Vector2f(100.0f * map.scale, 0.0f); //set new offset
			sprite.move(rotationPosOffset); //apply new offset
		}

		//===== next instruction is LEFT and is valid =====//
		else if (nextMoveDirInstruction == Direction::Left && map.GetMapDataAt(sf::Vector2i(mapLocation.x - 1, mapLocation.y)) == 0)
		{
			currentMoveDir = nextMoveDirInstruction; //set the next move instruction as the current followed instruction
			sprite.setRotation(180);
			sprite.move(-rotationPosOffset); //remove the current rotation offset from position
			rotationPosOffset = sf::Vector2f(100.0f * map.scale, 100.0f * map.scale); //set new offset
			sprite.move(rotationPosOffset); //apply new offset
		}

		//===== next instruction is RIGHT and is valid =====//
		else if (nextMoveDirInstruction == Direction::Right && map.GetMapDataAt(sf::Vector2i(mapLocation.x + 1, mapLocation.y)) == 0)
		{
			currentMoveDir = nextMoveDirInstruction; //set the next move instruction as the current followed instruction
			sprite.setRotation(0);
			sprite.move(-rotationPosOffset); //remove the current rotation offset from position
			rotationPosOffset = sf::Vector2f(0, 0); //set new offset
			//no need to apply since new offset == 0
		}

		//given instruction is not valid: check if we can keep moving in previous direction or if we should stop
		else if ((currentMoveDir == Direction::Up && map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y - 1)) == 1) ||
			(currentMoveDir == Direction::Down && map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y + 1)) == 1) ||
			(currentMoveDir == Direction::Left && map.GetMapDataAt(sf::Vector2i(mapLocation.x - 1, mapLocation.y)) == 1) ||
			(currentMoveDir == Direction::Right && map.GetMapDataAt(sf::Vector2i(mapLocation.x + 1, mapLocation.y)) == 1))
			currentMoveDir = Direction::None; //stop
	}

	prevSquareMidPos = sprite.getPosition(); //set prevSquareMidPos to new pos, as we are now at a new mid-square. Has to be done last due to the offsets
	//std::cout << "LOCATION: " << mapLocation.x << ", " << mapLocation.y << std::endl;
}

void Character::HandleAnimation()
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
}