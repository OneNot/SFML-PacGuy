#include "Map.h"

int Map::numOfPathSquares = 0;

Map::Map(sf::RenderWindow& renderWindow)
{
	if (!texture.loadFromFile("Assets/Sprites/pac-guy_map.png"))
	{
		std::cout << "Failed to load: Assets/Sprites/pac-guy_map.png" << std::endl;
		//window.close(); //TODO: error handling
	}

	//generate array map from map image
	sf::Image mapImg = texture.copyToImage();
	for (int x = 0; x < 19; x++)
	{
		for (int y = 0; y < 19; y++)
		{
			//std::cout << "at " << y << x << ": ";
			data[y][x] = (static_cast<int>(mapImg.getPixel(50 + y * 100, 50 + x * 100).a) == 0 ? 0 : 1);
			if (data[y][x] == 0)
				numOfPathSquares++;
			std::cout << (data[y][x] == 0 ? " " : "#");
		}
		std::cout << std::endl;
	}
	
	texture.setSmooth(true);
	sprite = sf::Sprite(texture);
	scale = (static_cast<float>(renderWindow.getSize().y) - 120) / static_cast<float>(texture.getSize().y);
	sprite.setScale(scale, scale);

	sf::FloatRect mapSpriteBounds(sprite.getLocalBounds());
	sprite.setPosition(renderWindow.getSize().x / 2 - (mapSpriteBounds.left + mapSpriteBounds.width * scale / 2),
		renderWindow.getSize().y / 2 - (mapSpriteBounds.top + mapSpriteBounds.height * scale / 2));

	scaledSquareSize = 100.0f * scale;

	mainSpawnMapLocation = sf::Vector2i(9, 9);
}

int Map::GetMapDataAt(sf::Vector2i pos)
{
	return data[pos.x][pos.y];
}

int Map::GetMapDataAt(int x, int y)
{
	return data[x][y];
}

sf::Vector2f Map::MapPosToRealPos(sf::Vector2i mapPos)
{
	return sf::Vector2f(
		sprite.getPosition().x + scaledSquareSize * mapPos.x,
		sprite.getPosition().y + scaledSquareSize * mapPos.y
	);
}

std::vector<sf::Vector2i> Map::GetValidNeighboursOf(sf::Vector2i square)
{
	std::vector<sf::Vector2i> valids = std::vector<sf::Vector2i>();
	if (data[square.x][square.y - 1] == 0)
		valids.push_back(sf::Vector2i(square.x, square.y - 1));
	if (data[square.x][square.y + 1] == 0)
		valids.push_back(sf::Vector2i(square.x, square.y + 1));
	if (data[square.x - 1][square.y] == 0)
		valids.push_back(sf::Vector2i(square.x - 1, square.y));
	if (data[square.x + 1][square.y] == 0)
		valids.push_back(sf::Vector2i(square.x + 1, square.y));

	return valids;
}



