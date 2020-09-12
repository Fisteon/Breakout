#include "UI.h"
#include "GameManager.h"

UI::UI() {
	if (!font.loadFromFile("Resources/Fonts/AGENCYB.TTF")) {
		return;
	}
	background.setFillColor(sf::Color(255, 255, 255, 80));
	background.setPosition(sf::Vector2f(0, 0));
	background.setSize(sf::Vector2f(
		GameManager::WINDOW_PLAY_AREA_WIDTH + GameManager::WINDOW_SIDEBAR_WIDTH,
		GameManager::WINDOW_HEIGHT));

	UIborder.setFillColor(sf::Color(255, 255, 255, 40));
	UIborder.setSize(sf::Vector2f(10, GameManager::WINDOW_HEIGHT));
	UIborder.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH, 0));

	score.setFont(font);
	score.setFillColor(sf::Color(170, 170, 170));
	score.setCharacterSize(34);
	score.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 100));

	level.setFont(font);
	level.setFillColor(sf::Color(170, 170, 170));
	level.setCharacterSize(34);
	level.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 150));

	pause.setFont(font);
	pause.setString("P - Pause");
	pause.setFillColor(sf::Color(170, 170, 170));
	pause.setCharacterSize(34);
	pause.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 300));

	newgame.setFont(font);
	newgame.setString("N - New Game");
	newgame.setFillColor(sf::Color(170, 170, 170));
	newgame.setCharacterSize(34);
	newgame.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 350));

	exit.setFont(font);
	exit.setString("X - Exit");
	exit.setFillColor(sf::Color(170, 170, 170));
	exit.setCharacterSize(34);
	exit.setPosition(sf::Vector2f(GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 400));

	status.setFont(font);
	status.setString("Press any key to start!");
	status.setFillColor(sf::Color(170, 170, 170));
	status.setCharacterSize(34);

	quadtree.setFont(font);
	quadtree.setString("Q - Show Quadtree");
	quadtree.setFillColor(sf::Color(170, 170, 170));
	quadtree.setCharacterSize(34);
	quadtree.setPosition(sf::Vector2f(
		GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 
		GameManager::WINDOW_HEIGHT - 50));

	autoplay.setFont(font);
	autoplay.setString("A - Autoplay");
	autoplay.setFillColor(sf::Color(170, 170, 170));
	autoplay.setCharacterSize(34);
	autoplay.setPosition(sf::Vector2f(
		GameManager::WINDOW_PLAY_AREA_WIDTH + 25, 
		GameManager::WINDOW_HEIGHT - 100));
	
	
	hearts.setSize(sf::Vector2f(25, 25));
	hearts.setTextureRect(sf::IntRect(50, 60, 50, 50));
}

void UI::repositionStatusText() {
	status.setPosition(sf::Vector2f(
		GameManager::WINDOW_PLAY_AREA_WIDTH / 2 - (status.getLocalBounds().width / 2),
		GameManager::WINDOW_HEIGHT / 2));
}