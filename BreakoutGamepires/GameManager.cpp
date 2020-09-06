#include "Utility.h"
#include "GameManager.h"
#include "LevelGenerator.h"


GameManager::GameManager(sf::RenderWindow* _window) {
	window = _window;
	window->create(sf::VideoMode(WINDOW_PLAY_AREA_WIDTH + WINDOW_SIDEBAR_WIDTH, WINDOW_HEIGHT), "Breakout");
	texture.loadFromFile("textures.png");

	// Ball setup
	ball = Ball(15.f, sf::Vector2f(250.f, 300.f), sf::Vector2f(15.f, 55.f));
	ball.setTexture(&texture);
	ball.setTextureRect(sf::IntRect(25, 0, 50, 50));
	ball.setOrigin(15.f, 15.f);

	// Background setup
	background.setFillColor(sf::Color(0, 0, 0, 0));
	background.setOutlineThickness(-5.f);
	background.setOutlineColor(sf::Color(255, 0, 0));
	background.setSize(sf::Vector2f(540, 600));

	// Level setup
	LevelGenerator levelGenerator;
	int levelNumber = 1;
	while (fileExists("LevelData_" + std::to_string(levelNumber) + ".xml")) {
		levels.push_back(levelGenerator.parseLevelInfo(levelNumber));
		levelNumber++;
	}
	initialLevelSetup(0);
	
	return;
}

void GameManager::Update(float deltaTime) {
	window->draw(background);

	for (Brick b : bricks) {
		window->draw(b);
	}
	//sf::Transform transform;
	//transform.rotate(55 * deltaTime, ball.getPosition() + sf::Vector2f(ball.getRadius()/2, ball.getRadius()/2));
	ball.rotate(90 * deltaTime);
	ball.move(ball.velocity * deltaTime);
	window->draw(ball);

	return;
}

void GameManager::initialLevelSetup(int levelNumber) {
	int rowCount = 0;
	for (std::string row : levels[levelNumber].levelLayout) {
		for (int i = 0; i < row.length(); i++) {
			if (row[i] == '_') continue;
			bricks.push_back(levels[levelNumber].brickTypes[row[i]]);
			bricks.back().setPosition(sf::Vector2f(
				10.f + i*(Brick::BRICK_WIDTH + levels[levelNumber].columnSpacing),
				10.f + rowCount*(Brick::BRICK_HEIGHT + levels[levelNumber].rowSpacing)));
			bricks.back().setSize(sf::Vector2f(25.f, 15.f));
			bricks.back().setTexture(&texture);
			bricks.back().setTextureRect(sf::IntRect(
				0, texturePositions.at(row[i]) * 15, Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT));
			window->draw(bricks.back());
		}
		rowCount++;
	}
}
