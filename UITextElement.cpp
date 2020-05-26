#include "UITextElement.h"

UITextElement::UITextElement(sf::RenderWindow& window, std::string string, sf::Font& font, sf::Color color, int size, UIAnchor anchor, sf::Vector2f offset)
{
	text = sf::Text(string, font, size);
	bounds = sf::FloatRect(text.getLocalBounds());
	text.setFillColor(color);

	switch (anchor)
	{
	case UIAnchor::TopLeft:
		text.setPosition(offset);
		break;
	case UIAnchor::TopMid:
		text.setPosition(
			window.getSize().x / 2 - (bounds.left + bounds.width / 2) + offset.x,
			offset.y
		);
		break;
	case UIAnchor::TopRight:
		text.setPosition(
			window.getSize().x - (bounds.left + bounds.width) - offset.x,
			offset.y
		);
		break;
	case UIAnchor::MidLeft:
		text.setPosition(
			offset.x,
			window.getSize().y / 2 - (bounds.top + bounds.height / 2) + offset.y
		);
		break;
	case UIAnchor::Mid:
		text.setPosition(
			window.getSize().x / 2 - (bounds.left + bounds.width / 2) + offset.x,
			window.getSize().y / 2 - (bounds.top + bounds.height / 2) + offset.y
		);
		break;
	case UIAnchor::MidRight:
		text.setPosition(
			window.getSize().x - (bounds.left + bounds.width) + offset.x,
			window.getSize().y / 2 - (bounds.top + bounds.height / 2) + offset.y
		);
		break;
	case UIAnchor::LowLeft:
		text.setPosition(
			offset.x,
			window.getSize().y - (bounds.top + bounds.height) + offset.y
		);
		break;
	case UIAnchor::LowMid:
		text.setPosition(
			window.getSize().x / 2 - (bounds.left + bounds.width / 2) + offset.x,
			window.getSize().y - (bounds.top + bounds.height) + offset.y
		);
		break;
	case UIAnchor::LowRight:
		text.setPosition(
			window.getSize().x - (bounds.left + bounds.width) + offset.x,
			window.getSize().y - (bounds.top + bounds.height) + offset.y
		);
		break;
	}
}

UITextElement::UITextElement(sf::RenderWindow& window, std::string string, sf::Font& font, sf::Color color, int size, UITextElement anchorToOtherElement, UIAnchor OtherElementEdgeAnchor, sf::Vector2f offset)
{
	text = sf::Text(string, font, size);
	bounds = sf::FloatRect(text.getLocalBounds());
	text.setFillColor(color);

	switch (OtherElementEdgeAnchor)
	{
	case UIAnchor::TopLeft:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top - bounds.height + offset.y
		);
		break;
	case UIAnchor::TopMid:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + anchorToOtherElement.bounds.width / 2 - bounds.width / 2 + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top - bounds.height + offset.y
		);
		break;
	case UIAnchor::TopRight:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + anchorToOtherElement.bounds.width - bounds.width + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top - bounds.height + offset.y
		);
		break;

	case UIAnchor::MidLeft:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left - bounds.width + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top + offset.y
		);
		break;
	case UIAnchor::MidRight:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + bounds.width + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top + offset.y
		);
		break;

	case UIAnchor::LowMid:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + anchorToOtherElement.bounds.width / 2 - bounds.width / 2 + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top + anchorToOtherElement.bounds.height + offset.y
		);
		break;
	case UIAnchor::LowLeft:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top + anchorToOtherElement.bounds.height + offset.y
		);
		break;
	case UIAnchor::LowRight:
		text.setPosition(
			anchorToOtherElement.text.getPosition().x + anchorToOtherElement.bounds.left + anchorToOtherElement.bounds.width - bounds.width + offset.x,
			anchorToOtherElement.text.getPosition().y + anchorToOtherElement.bounds.top + anchorToOtherElement.bounds.height + offset.y
		);
		break;
	}
}
