#include "Enemy.h"

std::vector<Enemy*> Enemy::enemies = std::vector<Enemy*>();

void Enemy::HandleNewMovementSquare(Map& map)
{
	Direction dir = GetNextDirection(map);
	switch (dir)
	{
	case Direction::Up:
		sprite.setRotation(-90);
		sprite.move(-rotationPosOffset); //remove the current rotation offset from position
		rotationPosOffset = sf::Vector2f(0.0f, 100.0f * map.scale); //set new offset
		sprite.move(rotationPosOffset); //apply new offset
		break;
	case Direction::Down:
		sprite.setRotation(90);
		sprite.move(-rotationPosOffset); //remove the current rotation offset from position
		rotationPosOffset = sf::Vector2f(100.0f * map.scale, 0.0f); //set new offset
		sprite.move(rotationPosOffset); //apply new offset
		break;
	case Direction::Left:
		sprite.setRotation(180);
		sprite.move(-rotationPosOffset); //remove the current rotation offset from position
		rotationPosOffset = sf::Vector2f(100.0f * map.scale, 100.0f * map.scale); //set new offset
		sprite.move(rotationPosOffset); //apply new offset
		break;
	case Direction::Right:
		sprite.setRotation(0);
		sprite.move(-rotationPosOffset); //remove the current rotation offset from position
		rotationPosOffset = sf::Vector2f(0, 0); //set new offset
		break;
	case Direction::None:
		break;
	}

	currentMoveDir = dir;
	prevSquareMidPos = sprite.getPosition(); //set prevSquareMidPos to new pos, as we are now at a new mid-square. Has to be done last due to the offsets
}

Enemy::Enemy(sf::RenderWindow& renderWindow, Map& map, std::string characterTextureToUse, sf::Vector2i spawnMapPosition) :
	Character(renderWindow, map, characterTextureToUse, spawnMapPosition)
{
	enemies.push_back(this);
}

Enemy::~Enemy()
{
	//TODO: remove from enemies vector
}
