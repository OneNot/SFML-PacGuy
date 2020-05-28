#include "Rando.h"

Direction Rando::GetNextDirection(Map& map)
{
	//valid directions
	std::vector<Direction> validDirs = std::vector<Direction>();

	if (map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y - 1)) == 0)
	{
		validDirs.push_back(Direction::Up);

		//quick and dirty way to make it so that turning 180 is less likely than other directions.
		//because each direction is inserted multiple times unless it's opposite to current move direction, in which case it's only inserted once
		//TODO: shoud do this better later. This is frankly disgusting code
		if (currentMoveDir != Direction::Down)
		{
			validDirs.push_back(Direction::Up);
			validDirs.push_back(Direction::Up);
			validDirs.push_back(Direction::Up);
			validDirs.push_back(Direction::Up);
		}
	}
	if (map.GetMapDataAt(sf::Vector2i(mapLocation.x, mapLocation.y + 1)) == 0)
	{
		validDirs.push_back(Direction::Down);
		if (currentMoveDir != Direction::Up)
		{
			validDirs.push_back(Direction::Down);
			validDirs.push_back(Direction::Down);
			validDirs.push_back(Direction::Down);
			validDirs.push_back(Direction::Down);
		}
	}
	if (map.GetMapDataAt(sf::Vector2i(mapLocation.x - 1, mapLocation.y)) == 0)
	{
		validDirs.push_back(Direction::Left);
		if (currentMoveDir != Direction::Right)
		{
			validDirs.push_back(Direction::Left);
			validDirs.push_back(Direction::Left);
			validDirs.push_back(Direction::Left);
			validDirs.push_back(Direction::Left);
		}
	}
	if (map.GetMapDataAt(sf::Vector2i(mapLocation.x + 1, mapLocation.y)) == 0)
	{
		validDirs.push_back(Direction::Right);
		if (currentMoveDir != Direction::Left)
		{
			validDirs.push_back(Direction::Right);
			validDirs.push_back(Direction::Right);
			validDirs.push_back(Direction::Right);
			validDirs.push_back(Direction::Right);
		}
	}

	if (validDirs.size() == 0)
		return Direction::None;

	return validDirs.at(rand() % validDirs.size());
}