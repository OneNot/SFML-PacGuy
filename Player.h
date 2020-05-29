#pragma once
#include "Character.h"
#include "Collectable.h"
#include "AudioManager.h"
#include <string>
#include "Enemy.h"

class Player :
	public Character
{
private:
	void HandleNewMovementSquare(Map& map) override;
	void CollisionCheck() override;

public:
	static Player* player;
	Player(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition = sf::Vector2i::Vector2(0, 0));
	void HandleAnimation() override;
};

