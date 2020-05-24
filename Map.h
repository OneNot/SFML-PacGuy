#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Map
{
private:
	int data[19][19];
	sf::Texture texture;

public:
	sf::Sprite sprite;
	Map(sf::RenderWindow &renderWindow);
	float scale;
	float scaledSquareSize;
	int GetMapDataAt(sf::Vector2i pos);
};