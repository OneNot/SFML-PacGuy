#include <SFML/Graphics.hpp>
#include "Player.h"

void Player::HandleNewMovementSquare(Map& map)
{
	//check for collectables
	Collectable::TryGetCollectableAtMapPos(*this, mapLocation);

	Character::HandleNewMovementSquare(map); //use base class new square handling
}

void Player::UpdateScore(int scoreChange)
{
	score += scoreChange;
	std::cout << "SCORE: " << score << std::endl;
}
