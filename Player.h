#pragma once
#include "Character.h"
#include "Collectable.h"
#include "AudioManager.h"
#include <string>

class Player :
	public Character
{
private:
	void HandleNewMovementSquare(Map& map) override;

public:
	using Character::Character;
	void HandleAnimation() override;
};

