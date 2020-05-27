#pragma once
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h> 
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <algorithm>

class Collectable
{
private:
	static std::string pelletImageFile;
	static sf::Texture pelletTexture;
	static std::string cherryImageFile;
	static sf::Texture cherryTexture;
	static std::vector<std::unique_ptr<Collectable>> collectables;
	int scoreValue = 10;

public:
	sf::Vector2i mapLocation;
	sf::Sprite sprite;
	static void GenerateCollectables(class Map& map);
	static void DrawCollectables(sf::RenderWindow& window);
	static void PrintCollectables();
	void Collect(int vectorPos);
	Collectable(class Map& map, sf::Vector2i spawnMapLocation);
	static bool TryGetCollectableAtMapPos(class Character& collector, sf::Vector2i mapPos);
	static void SwapCollectablesForCherry(int howMany);
};

