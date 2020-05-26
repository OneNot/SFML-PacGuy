#pragma once
#include "Character.h"
#include "Collectable.h"

class Player :
	public Character
{
private:
	int score = 0;
	void HandleNewMovementSquare(Map& map) override;

public:
	using Character::Character;
	void UpdateScore(int scoreChange) override;
};

