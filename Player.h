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
	using Character::Character;
	void HandleAnimation() override;
};

