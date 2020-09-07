#include "Utility.h"
#include "GameManager.h"
#include "LevelGenerator.h"
#include "QuadTree.h"

bool keyReleased = true;
int brickdelete = 59;

GameManager::GameManager(sf::RenderWindow* _window) {
	window = _window;
	window->create(sf::VideoMode(WINDOW_PLAY_AREA_WIDTH + WINDOW_SIDEBAR_WIDTH, WINDOW_HEIGHT), "Breakout");
	texture.loadFromFile("textures.png");

	// Ball setup
	ball = Ball(10.f, sf::Vector2f(250.f, 300.f), sf::Vector2f(15.f, -55.f));
	ball.setTexture(&texture);
	ball.setTextureRect(sf::IntRect(25, 0, 50, 50));
	ball.setOrigin(10.f, 10.f);

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

	// QuadTree setup
	qt = QuadTree(0.f, 0.f, WINDOW_PLAY_AREA_WIDTH, WINDOW_HEIGHT, 0);
	for (int i = 0; i < bricks.size(); i++){
		qt.addBrick(&bricks[i]);
	}
	
	return;
}

void GameManager::drawQTshapes(QuadTree* qt) {
	window->draw(qt->bounds);
	if (qt->NW != nullptr) drawQTshapes(qt->NW);
	if (qt->NE != nullptr) drawQTshapes(qt->NE);
	if (qt->SW != nullptr) drawQTshapes(qt->SW);
	if (qt->SE != nullptr) drawQTshapes(qt->SE);
}

void GameManager::update(float deltaTime) {
	//window->draw(background);	

	for (Brick b : bricks) {
		if (b.hp != 0) {
			window->draw(b);
		}
	}
	drawQTshapes(&qt);
	for (Brick* b : qt.collisionBricks(ball)) {
		std::cout << b->hp;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && keyReleased) {
		// delete brick X;
		keyReleased = false;
		deleteBrick(brickdelete--);
		std::cout << "Pressed: A";
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		keyReleased = true;
	}

	ball.rotate(90 * deltaTime);
	ball.move(ball.velocity * deltaTime);
	window->draw(ball);

	return;
}

void GameManager::deleteBrick(int i) {
	bricks[i].hp = 0;
	qt.deleteBrick(&bricks[i]);
}

void GameManager::initialLevelSetup(int levelNumber) {
	int rowCount = 0;
	for (std::string row : levels[levelNumber].levelLayout) {
		for (int i = 0; i < row.length(); i++) {
			if (row[i] == '_') continue;
			bricks.push_back(levels[levelNumber].brickTypes[row[i]]);
			bricks.back().setSize(sf::Vector2f(Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT));
			bricks.back().setTexture(&texture);
			bricks.back().setTextureRect(sf::IntRect(
				0, 
				texturePositions.at(row[i]) * 15, 
				Brick::BRICK_WIDTH, 
				Brick::BRICK_HEIGHT));
			bricks.back().setPosition(sf::Vector2f(
				10.f + i*(Brick::BRICK_WIDTH + levels[levelNumber].columnSpacing),
				10.f + rowCount*(Brick::BRICK_HEIGHT + levels[levelNumber].rowSpacing)));
			window->draw(bricks.back());
			std::cout << bricks.back().getGlobalBounds().top << " " << bricks.back().getGlobalBounds().left << " " << bricks.back().getGlobalBounds().width << " " << bricks.back().getGlobalBounds().height << "\n";
		}
		rowCount++;
	}
}
