#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

/*
Some planning

- 2d array of squares (bool): wall or not wall. used for checking "collisions" or rather, allowed movement
- You don't constantly hold a direction to go. Pac-guy moves in a direction until it hits a wall or is given a new direction
- Movement states: middle of square or not
- new directions are applied at mid-square (turning fully around might be allowed even if not mid-square)
- When a direction input is given, it gets queued to be used asap
- If a new direction is given it overrides the previous instruction
- At every square, game checks if it is possible to move according to the last instruction
- If pac-guy hits a wall (e.g. if the last instruction is up when moving up to a T intersection), then pac-guy stops (maybe even gets stunned?)
- directions are absolute, not in relation to pac-guy

With this system it might be feasable to make a simple map builder or even a random generator. Though that is not part of the plan atm


Basic movement scenario example:
1. pac-guy is moving up and is in between squares. In 3 squares there is a T intersection
2. player presses right and the instruction gets recorded
4. next mid-square is reached, the game checks if it's possible to turn right. It isn't, so nothing happens and the pac-guy keeps moving up
5. this continues until the intersection is reached
6. Just before the intersection, the player presses left and the previous instruction gets replaced
7. pac-guy reaches the mid-square of the intersection. The game checks if it's possible to turn left. It is, so pac guy turns left and keeps moving

********
Movement sort of working now but allowing any direction from any square.
TODO:
- Need to combine the 2D array with the system to check what directions are allowed from any given square (Might try to code a little something that will create the array from the map image itself...)
- Collectables
- Enemies
- Edge teleport
- Animating
- Audio
- Need to do a shit-ton of code cleanup and refactoring
*/


enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pac-Guy");

	float moveSpeed = 100.0f;

	//y, x
	int map[11][11] =
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};


#pragma region Loading assets and creating objects

	// Create a font object and load it from file relative
	sf::Font font;
	if (!font.loadFromFile("Assets/Fonts/impact.ttf"))
	{
		std::cout << "Failed to load: Assets/Fonts/impact.ttf" << std::endl;
		return 42;
	}



	//Create Hello World text object using our font and size 128pt
	sf::Text gameTitleText("Pac-Guy", font, 48);
	gameTitleText.setFillColor(sf::Color::Red); //set text color to red

	//Get the text object's physical dimensions and use them to center the text to our render window
	//By default things are drawn relative to their top left corner and can be changed by calling setOrigin()
	sf::FloatRect gameTitleTextBounds(gameTitleText.getLocalBounds());
	gameTitleText.setPosition(window.getSize().x / 2 - (gameTitleTextBounds.left + gameTitleTextBounds.width / 2), 0);




	//create a Texture, load it and make a Sprite out of it
	sf::Texture mapTexture;
	if (!mapTexture.loadFromFile("Assets/Sprites/pac-guy_map.png"))
	{
		std::cout << "Failed to load: Assets/Sprites/pac-guy_map.png" << std::endl;
		window.close();
	}
	mapTexture.setSmooth(true);
	sf::Sprite mapSprite(mapTexture);
	float scale = (static_cast<float>(window.getSize().y) - 120) / static_cast<float>(mapTexture.getSize().y);
	mapSprite.setScale(scale, scale);

	sf::FloatRect mapSpriteBounds(mapSprite.getLocalBounds());
	mapSprite.setPosition(window.getSize().x / 2 - (mapSpriteBounds.left + mapSpriteBounds.width * scale / 2),
		window.getSize().y / 2	- (mapSpriteBounds.top + mapSpriteBounds.height * scale / 2));
	//mapSprite.setPosition(0, 0);



	sf::Texture pacGuyTexture;
	if (!pacGuyTexture.loadFromFile("Assets/Sprites/pac-guy.png"))
	{
		std::cout << "Failed to load: Assets/Sprites/pac-guy.png" << std::endl;
		window.close();
	}
	pacGuyTexture.setSmooth(true);
	sf::Sprite pacGuySprite(pacGuyTexture);
	pacGuySprite.setScale(scale, scale);
	sf::FloatRect pacGuySpriteBounds(pacGuySprite.getLocalBounds());
	pacGuySprite.setPosition(window.getSize().x / 2 - (pacGuySpriteBounds.left + pacGuySpriteBounds.width * scale / 2),
		window.getSize().y / 2 - (pacGuySpriteBounds.top + pacGuySpriteBounds.height * scale / 2));

#pragma endregion


	float delta = 0.0f;
	sf::Clock frameTimeClock;

	//for framerate
	sf::Clock frameRateClock;
	int frameCount = 0;

	Direction dirInstruction = Direction::None, currentMoveDir = Direction::None;

	sf::Vector2f prevSquare = pacGuySprite.getPosition();
	float moved = 0.0f;
	float scaledSquareSize = 100.0f * scale;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				window.close();

			//Event driven input handling
			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
					dirInstruction = Direction::Left;
				if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
					dirInstruction = Direction::Right;
				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
					dirInstruction = Direction::Up;
				if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
					dirInstruction = Direction::Down;
			}
		}

		//std::cout << "dir: " << static_cast<int>(dirInstruction) << std::endl;
		switch (currentMoveDir)
		{
			case Direction::Down:
				pacGuySprite.move(0.0f, delta * moveSpeed);
				moved += delta * moveSpeed;
				std::cout << "DOWN " << std::endl;
				if (pacGuySprite.getPosition().y >= prevSquare.y + scaledSquareSize)
				{
					pacGuySprite.setPosition(pacGuySprite.getPosition().x, prevSquare.y + scaledSquareSize);
					currentMoveDir = dirInstruction;
					moved = 0.0f;
					prevSquare = pacGuySprite.getPosition();
				}
				break;
			case Direction::Up:
				pacGuySprite.move(0.0f, -delta * moveSpeed);
				moved += delta * moveSpeed;
				std::cout << "UP" << std::endl;
				if (pacGuySprite.getPosition().y <= prevSquare.y - scaledSquareSize)
				{
					pacGuySprite.setPosition(pacGuySprite.getPosition().x, prevSquare.y - scaledSquareSize);
					currentMoveDir = dirInstruction;
					moved = 0.0f;
					prevSquare = pacGuySprite.getPosition();
				}
				break;
			case Direction::Left:
				pacGuySprite.move(-delta * moveSpeed, 0.0f);
				moved += delta * moveSpeed;
				std::cout << "LEFT" << std::endl;
				if (pacGuySprite.getPosition().x <= prevSquare.x - scaledSquareSize)
				{
					pacGuySprite.setPosition(prevSquare.x - scaledSquareSize, pacGuySprite.getPosition().y);
					currentMoveDir = dirInstruction;
					moved = 0.0f;
					prevSquare = pacGuySprite.getPosition();
				}
				break;
			case Direction::Right:
				pacGuySprite.move(delta * moveSpeed, 0.0f);
				moved += delta * moveSpeed;
				std::cout << "RIGHT" << std::endl;
				if (pacGuySprite.getPosition().x >= prevSquare.x + scaledSquareSize)
				{
					pacGuySprite.setPosition(prevSquare.x + scaledSquareSize, pacGuySprite.getPosition().y);
					currentMoveDir = dirInstruction;
					moved = 0.0f;
					prevSquare = pacGuySprite.getPosition();
				}
				break;

			default:
			case Direction::None:
				currentMoveDir = dirInstruction;
				break;
		}

		//tutorial I'm loosely following placed the frame timer restart here...
		//not sure why, I would think it makes more sense to include the drawing and displaying in the frame time?
		//so I'm putting it after them for now

		window.clear(sf::Color::Color(48, 25, 52, 255));
		window.draw(gameTitleText);
		window.draw(mapSprite);
		window.draw(pacGuySprite);
		window.display();

		//get frame time
		delta = static_cast<float>(frameTimeClock.getElapsedTime().asMicroseconds()) / 1000000.0f;
		//std::cout << "delta (ms): " << delta << std::endl;
		frameTimeClock.restart();
		frameCount++;

		//get framerate
		if (frameRateClock.getElapsedTime().asSeconds() >= 1.0f)
		{
			std::cout << frameCount << " FPS" << std::endl;
			frameRateClock.restart();
			frameCount = 0;
		}
	}

	return 0;
}