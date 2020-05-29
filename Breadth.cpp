#include "Breadth.h"

Direction Breadth::GetNextDirection(Map& map)
{
	//currently the path is re-calculated in every square...
	std::vector<sf::Vector2i> path = GetPathToPlayer(map);

	//if the first square in the path is not self, a path could not be found and likely something went wrong...
	//or if the path contains only the starting square, the Enemy thinks he's at Player location (shouldn't happen, but just in case)
	if (path.size() < 2 || path[0] != mapLocation)
		return Direction::None;
	
	//return the direction of the next square in the path
	if (path[1] == sf::Vector2i(mapLocation.x, mapLocation.y - 1))
		return Direction::Up;
	else if (path[1] == sf::Vector2i(mapLocation.x, mapLocation.y + 1))
		return Direction::Down;
	else if (path[1] == sf::Vector2i(mapLocation.x - 1, mapLocation.y))
		return Direction::Left;
	else if (path[1] == sf::Vector2i(mapLocation.x + 1, mapLocation.y))
		return Direction::Right;
}

std::vector<sf::Vector2i> Breadth::GetPathToPlayer(Map& map)
{
	std::vector<std::vector<sf::Vector2i>> parents = solveBFS(map);
	std::vector<sf::Vector2i> path = std::vector<sf::Vector2i>();

	//Get the target square
	//Since mapLocation stored by Player is of the last square the player was in, we need to do a little extra so we can target the square the player is actually GOING to
	sf::Vector2i targetPos = Player::player->mapLocation;
	switch (Player::player->currentMoveDir)
	{
	case Direction::Up:
		targetPos += sf::Vector2i(0, -1);
		break;
	case Direction::Down:
		targetPos += sf::Vector2i(0, 1);
		break;
	case Direction::Left:
		targetPos += sf::Vector2i(-1, 0);
		break;
	case Direction::Right:
		targetPos += sf::Vector2i(1, 0);
		break;
	}

	//construct path Player -> Enemy
	//start with target(player); loop until we run into a square that has no parent on record (hopefully because we reached enemy position); set the parent of current square as the next "current" for next iteration
	for (sf::Vector2i current = targetPos; current != sf::Vector2i(-1, -1); current = parents[current.x][current.y])
		path.push_back(current);

	std::reverse(path.begin(), path.end()); //reverse path so it's Enemy -> Player

	return path;
}

std::vector<std::vector<sf::Vector2i>> Breadth::solveBFS(Map& map)
{
	std::queue<sf::Vector2i> q; //initialize queue
	q.push(mapLocation); //add own position as starting point

	//TODO: NOTE!!! currently hardcoded for 19x19 map
	std::vector<std::vector<bool>> visited(19, std::vector<bool>(19, false)); //2D bool vector to keep track of visited squares
	visited[mapLocation.x][mapLocation.y] = true; //mark starting square as visited
	std::vector<std::vector<sf::Vector2i>> parents(19, std::vector<sf::Vector2i>(19, sf::Vector2i(-1, -1))); //vector for keeping track of the parent square of each visited square (-1,-1 represents unset value)
	
	//loop until queue is empty
	while (!q.empty())
	{
		sf::Vector2i currentSquare = q.front(); //operate on the next square in the queue
		q.pop(); //the square can be dequeued

		std::vector<sf::Vector2i> validNeighbours = map.GetValidNeighboursOf(currentSquare); //get valid neighbour squares
		for (int i = 0; i < validNeighbours.size(); i++) //loop through the valid neighbours
		{
			if (!visited[validNeighbours[i].x][validNeighbours[i].y]) //if the valid neighbour hasn't been visited yet...
			{
				q.push(validNeighbours[i]); //add to queue
				visited[validNeighbours[i].x][validNeighbours[i].y] = true; //mark it as visited
				parents[validNeighbours[i].x][validNeighbours[i].y] = currentSquare; //mark current square as the parent
			}
		}
	}
	return parents;
}
