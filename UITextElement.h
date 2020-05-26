#pragma once
#include <SFML/Graphics.hpp>
#include "UIAnchor.h"
#include <string>

class UITextElement
{
public:
	UITextElement(sf::RenderWindow& window, std::string string, sf::Font& font, sf::Color color, int size, UIAnchor anchor, sf::Vector2f offset = sf::Vector2f(0, 0));
	UITextElement(sf::RenderWindow& window, std::string string, sf::Font& font, sf::Color color, int size, UITextElement anchorToOtherElement, UIAnchor OtherElementEdgeAnchor, sf::Vector2f offset = sf::Vector2f(0, 0));

	sf::Text text;
	sf::FloatRect bounds;
};

