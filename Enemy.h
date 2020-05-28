#pragma once
#include "Character.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

class Enemy :
	public Character
{
private:
	void HandleNewMovementSquare(Map& map) override;

protected:
	virtual Direction GetNextDirection(Map& map) { return Direction::None; };

public:
	static std::vector<Enemy*> enemies;
	Enemy(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition = sf::Vector2i::Vector2(0, 0));
	~Enemy();
};

