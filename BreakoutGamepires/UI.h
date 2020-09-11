#pragma once

#include <string>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

class UI {
public:
	UI();
	void repositionStatusText();

	sf::RectangleShape hearts;
	sf::RectangleShape UIborder;
	sf::RectangleShape scoreboard;
	sf::RectangleShape background;
	sf::Texture backgroundTexture;

	sf::Font font;

	sf::Text status;
	sf::Text lives;
	sf::Text score;
	sf::Text level;

	sf::Text pause;
	sf::Text newgame;
	sf::Text exit;

	sf::Text quadtree;
	sf::Text autoplay;
};