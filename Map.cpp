#include "Map.h"

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
}

int Map::GetMapDataAt(sf::Vector2i pos)
{
	return data[pos.x][pos.y];
}
