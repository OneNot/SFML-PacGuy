#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <typeinfo>
#include <algorithm>

class Collectable
{
private:
	static sf::Texture texture;
	static bool textureSet;
	static std::vector<std::unique_ptr<Collectable>> collectables;
	int scoreValue = 10;

public:
	sf::Vector2i mapLocation;
	sf::Sprite sprite;
	std::string imageFile = "Assets/Sprites/collectable.png";
	static void GenerateCollectables(class Map& map);
	static void DrawCollectables(sf::RenderWindow& window);
	static void PrintCollectables();
	void Collect(int vectorPos);
	Collectable(class Map& map, sf::Vector2i spawnMapLocation);
	static bool TryGetCollectableAtMapPos(class Character& collector, sf::Vector2i mapPos);
};

