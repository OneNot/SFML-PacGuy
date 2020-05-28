#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Direction.h"
#include "Map.h"
#include "GameManager.h"
//#include "AudioManager.h"

class Character
{
private:
	sf::Texture texture;
	sf::Vector2f rotationPosOffset;

protected:
	virtual void HandleNewMovementSquare(Map& map);
	sf::Clock animationClock;
	int currentAnimationFrame;
	sf::IntRect rectSource;

public:
	sf::Sprite sprite;
	Character(sf::RenderWindow &renderWindow, Map &map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition = sf::Vector2i::Vector2(0, 0));
	float moveSpeed = 100.0f;
	sf::Vector2f prevSquareMidPos;
	float movedSinceLastSquareMid = 0.0f;
	void MovementHandling(Map &map, float delta);
	Direction currentMoveDir = Direction::None;
	Direction nextMoveDirInstruction = Direction::None;
	sf::Vector2i mapLocation;
	int numOfAnimationFrames = 3;
	virtual void HandleAnimation() {};
};
