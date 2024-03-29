#include "Collectable.h"
#include "Map.h"
#include "Character.h"

//apparently static member declarations need to be seperately defined, so here we go.
std::vector<std::unique_ptr<Collectable>> Collectable::collectables = std::vector<std::unique_ptr<Collectable>>();
sf::Texture Collectable::pelletTexture;
std::string  Collectable::pelletImageFile = "Assets/Sprites/collectable.png";
sf::Texture Collectable::cherryTexture;
std::string  Collectable::cherryImageFile = "Assets/Sprites/cherry_100_2.png";

Collectable::Collectable(Map &map, sf::Vector2i spawnMapLocation)
{
	//real spawn location is map position + map position * map square size
	//i.e. at map position 0 -> map position + 0 * square size = map position -> 
	sf::Vector2f realSpawnLocation = map.sprite.getPosition() + sf::Vector2f((map.scaledSquareSize * spawnMapLocation.x),(map.scaledSquareSize * spawnMapLocation.y));

	sprite = sf::Sprite(pelletTexture);
	sprite.setScale(map.scale, map.scale);
	sprite.setPosition(realSpawnLocation);
	mapLocation = spawnMapLocation;

	GameManager::TotalNumOfCollectables++;

	//std::cout << "spawnPos: " << realSpawnLocation.x << "," << realSpawnLocation.y << std::endl;
	//std::cout << "spawnPos: " <<  mapLocation.x << "," << mapLocation.y << std::endl;
}

void Collectable::GenerateCollectables(Map &map)
{
	if (!pelletTexture.loadFromFile(pelletImageFile))
	{
		std::cout << "Failed to load: " << pelletImageFile << std::endl;
		//window.close(); //TODO: error handling
	}
	pelletTexture.setSmooth(true);

	if (!cherryTexture.loadFromFile(cherryImageFile))
	{
		std::cout << "Failed to load: " << cherryImageFile << std::endl;
		//window.close(); //TODO: error handling
	}
	cherryTexture.setSmooth(true);

	//collectables.clear();
	for (int y = 0; y < 19; y++)
		for (int x = 0; x < 19; x++)
			if (map.GetMapDataAt(x, y) == 0 &&
				sf::Vector2i(x, y) != map.mainSpawnMapLocation &&
				sf::Vector2i(x, y) != map.teleports[0] &&
				sf::Vector2i(x, y) != map.teleports[1])
				collectables.push_back(std::make_unique<Collectable>(map, sf::Vector2i(x, y)));

	SwapCollectablesForCherry(10);

	//I dont fully understand how unique pointers work.
	//It would seem to make sense to me that this should work with a list of normal pointers just as well:
	// ^ When the Constructor is called, store a pointer to the object generated. Seems simple enough.
	//   Yet, when the constructor is called a second time and a new object is created, it seems that object is written over the previous one???
	//   Or at least, after using push_back() on the list for both pointers, the list then contains two identical addresses...

	//Using unique pointers, this problem is no more.
	//Also, even though we currently don't want to allow constructing collectables outside of this method, the constructor can no longer be private or make_unique can't access it...
	//I'm sure there is a workaround for that, but I'm confused enough as it is...

	//PrintCollectables();
}

void Collectable::PrintCollectables()
{
	std::cout << "===== collectables =====" << std::endl;
	for (int i = 0; i < collectables.size(); i++)
		std::cout << "map location: " << collectables.at(i)->mapLocation.x << "," << collectables.at(i)->mapLocation.y << std::endl;
		//std::cout << "pointer: " << collectables.at(i) << std::endl;
}

void Collectable::DrawCollectables(sf::RenderWindow& window)
{
	for (int i = 0; i < collectables.size(); i++)
	{
		//std::cout << "map location: " << collectables.at(i)->mapLocation.x << "," << collectables.at(i)->mapLocation.y << std::endl;
		window.draw(collectables.at(i)->sprite);
	}
}

bool Collectable::TryGetCollectableAtMapPos(Character& collector, sf::Vector2i mapPos)
{
	//There might be something equivelant to C# Find(x => x.mapLocation) in C++, but I'm going to just do this "manually" for now

	for (int i = 0; i < collectables.size(); i++)
	{
		if (collectables.at(i)->mapLocation == mapPos)
		{
			collectables.at(i)->Collect(i);
			return true;
		}
	}
	return false;
}

void Collectable::SwapCollectablesForCherry(int howMany)
{
	//srand(time(NULL));

	for (int i = 0; i < howMany; i++)
	{
		Collectable& c = *collectables.at(rand() % collectables.size());
		c.sprite.setTexture(cherryTexture);
		c.scoreValue = 50;
	}
}

void Collectable::WipeCollectables()
{
	collectables.clear();
}

void Collectable::Collect(int vectorPos)
{
	if (scoreValue > 10)
		AudioManager::sounds["eat_fruit"]->sound.play();

	GameManager::AddToCollected();
	GameManager::UpdateScore(scoreValue);
	collectables.erase(collectables.begin() + vectorPos); //is this enough? Does this cause memory leaks and shit?
}

