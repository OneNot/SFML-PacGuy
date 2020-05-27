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

public:
	sf::Vector2i teleports[2] = { sf::Vector2i(0, 9), sf::Vector2i(18, 9) };
	sf::Sprite sprite;
	Map(sf::RenderWindow &renderWindow);
	float scale;
	float scaledSquareSize;
	int GetMapDataAt(sf::Vector2i pos);
	int GetMapDataAt(int x, int y);
	sf::Vector2i mainSpawnMapLocation;
	sf::Vector2f MapPosToRealPos(sf::Vector2i mapPos);
};