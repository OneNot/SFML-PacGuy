#pragma once
#include "Character.h"
#include "Collectable.h"

class Player :
	public Character
{
private:
	void HandleNewMovementSquare(Map& map) override;

public:
	using Character::Character;
};

