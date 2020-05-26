#pragma once
#include <iostream>
//#include <list>
#include <SFML/Graphics.hpp>
#include "Collectable.h"

class Map
{
private:
	int data[19][19];
	sf::Texture texture;
	//std::list<Collectable> collectables;

public:
	sf::Sprite sprite;
	Map(sf::RenderWindow &renderWindow);
	float scale;
	float scaledSquareSize;
	int GetMapDataAt(sf::Vector2i pos);
	int GetMapDataAt(int x, int y);
	sf::Vector2i mainSpawnMapLocation;
};