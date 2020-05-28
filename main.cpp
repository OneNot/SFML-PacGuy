#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Direction.h"
#include "Map.h"
#include "Player.h"
#include "Rando.h"
#include "Collectable.h"
#include "UITextElement.h"
#include "AudioManager.h"

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

**************************************
**************************************

Notes for next time:
- Enemies
	- Rando: moves nearly randomly
- Player <-> Enemy collision
- Death and Lose Game conditions

TODO:
- More Enemies
- Need to do a little bit of code cleanup and refactoring
*/


int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pac-Guy");

	#pragma region Loading assets and creating objects

		// Create a font object and load it from file relative
		sf::Font font;
		if (!font.loadFromFile("Assets/Fonts/impact.ttf"))
		{
			std::cout << "Failed to load: Assets/Fonts/impact.ttf" << std::endl;
			return 42;
		}

		#pragma region Creating UI Elements
			UITextElement gameTitleText = UITextElement(window, "Pac-Guy", font, sf::Color::Yellow, 48, UIAnchor::TopMid);

			UITextElement startText = UITextElement(window, "START GAME", font, sf::Color::Yellow, 128, UIAnchor::Mid);
			UITextElement startSubText = UITextElement(window, "Space/Enter to start", font, sf::Color::Yellow, 32, startText, UIAnchor::LowMid);

			UITextElement pauseText = UITextElement(window, "PAUSE", font, sf::Color::Yellow, 128, UIAnchor::Mid);
			UITextElement pauseSubText = UITextElement(window, "Press ESC to unpause", font, sf::Color::Yellow, 32, pauseText, UIAnchor::LowMid);

			UITextElement loseText = UITextElement(window, "YOU LOSE!", font, sf::Color::Red, 128, UIAnchor::Mid);
			UITextElement loseSubText = UITextElement(window, "Score:", font, sf::Color::White, 64, loseText, UIAnchor::LowMid);

			UITextElement winText = UITextElement(window, "YOU WIN!", font, sf::Color::Green, 128, UIAnchor::Mid);
			UITextElement winSubText = UITextElement(window, "Score", font, sf::Color::White, 64, winText, UIAnchor::LowMid);
			UITextElement winLoseScoreText = UITextElement(window, "0000000", font, sf::Color::White, 64, winSubText, UIAnchor::LowMid);
			UITextElement winLoseSubText2 = UITextElement(window, "No restart implemented currently. Please restart program.", font, sf::Color::White, 20, winLoseScoreText, UIAnchor::LowMid);
			GameManager::UIWinScore = &winLoseScoreText;

			UITextElement UIScoreText = UITextElement(window, "Score: 0", font, sf::Color::Yellow, 25, UIAnchor::TopLeft, sf::Vector2f(5, 5));
			GameManager::UIScore = &UIScoreText;
			UITextElement UICollectText = UITextElement(window, "Collected: 0", font, sf::Color::Yellow, 25, UIScoreText, UIAnchor::LowLeft, sf::Vector2f(0, 5));
			GameManager::UICollected = &UICollectText;
		#pragma endregion


		AudioManager::InitializeAudio();

		//Load and initialize map
		Map map = Map::Map(window);

		Collectable::GenerateCollectables(map);

		//Load and initialize player
		Player player = Player(window, map, "Assets/Sprites/pac-guys_spritesheet.png");

		Rando rando = Rando(window, map, "Assets/Sprites/ghost1_spritesheet.png", sf::Vector2i(1, 1));

		player.moveSpeed = 400.0f * map.scale;
		rando.moveSpeed = 400.0f * map.scale;

	#pragma endregion


	float delta = 0.0f;
	sf::Clock frameTimeClock;

	//for framerate
	/*sf::Clock frameRateClock;
	int frameCount = 0;*/

	sf::Clock scoreLossClock;
	GameManager::scoreLossClock = &scoreLossClock;

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
				if (event.key.code == sf::Keyboard::Escape)
					GameManager::TogglePauseGame();

				//allow movement inputs if the game isnt paused, won or lost
				if (GameManager::GetGameState() == GameState::Playing)
				{
					if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
						player.nextMoveDirInstruction = Direction::Left;
					if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
						player.nextMoveDirInstruction = Direction::Right;
					if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
						player.nextMoveDirInstruction = Direction::Up;
					if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
						player.nextMoveDirInstruction = Direction::Down;
				}
				else if (GameManager::GetGameState() == GameState::Menu &&
					(event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter))
					GameManager::ForceSetGameState(GameState::Playing);
			}
		}

		if (GameManager::GetGameState() == GameState::Playing)
		{
			player.MovementHandling(map, delta);
			rando.MovementHandling(map, delta);
			GameManager::HandleScoreLossTimer();
		}

		//tutorial I'm loosely following placed the frame timer restart here...
		//not sure why, I would think it makes more sense to include the drawing and displaying in the frame time?
		//so I'm putting it after them for now

		#pragma region Draw and display
			window.clear(sf::Color::Color(48, 25, 52, 255));
			window.draw(gameTitleText.text);
			window.draw(map.sprite);
			Collectable::DrawCollectables(window);
			window.draw(player.sprite);
			window.draw(rando.sprite);

			if (GameManager::GetGameState() == GameState::Menu)
			{
				window.draw(startText.text);
				window.draw(startSubText.text);
			}
			else if (GameManager::GetGameState() == GameState::Paused)
			{
				window.draw(pauseText.text);
				window.draw(pauseSubText.text);
			}
			else if (GameManager::GetGameState() == GameState::Playing)
			{
				window.draw(UIScoreText.text);
				window.draw(UICollectText.text);
			}
			else if (GameManager::GetGameState() == GameState::Won)
			{
				window.draw(winText.text);
				window.draw(winSubText.text);
				window.draw(winLoseScoreText.text);
				window.draw(winLoseSubText2.text);
			}
			else if (GameManager::GetGameState() == GameState::Lost)
			{
				window.draw(loseText.text);
				window.draw(loseSubText.text);
				window.draw(winLoseScoreText.text);
				window.draw(winLoseSubText2.text);
			}

			window.display();
		#pragma endregion

		//get frame time
		delta = static_cast<float>(frameTimeClock.getElapsedTime().asMicroseconds()) / 1000000.0f;
		//std::cout << "delta (ms): " << delta << std::endl;
		frameTimeClock.restart();

		//frameCount++;

		//get framerate
		/*if (frameRateClock.getElapsedTime().asSeconds() >= 1.0f)
		{
			std::cout << frameCount << " FPS" << std::endl;
			frameRateClock.restart();
			frameCount = 0;
		}*/
	}

	return 0;
}