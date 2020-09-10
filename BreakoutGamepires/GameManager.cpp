#include "Utility.h"
#include "GameManager.h"
#include "LevelGenerator.h"
#include "QuadTree.h"

Brick player = Brick(sf::Vector2f(245.f, 575.f), sf::Vector2f(100.f, 15.f), "", "");

GameManager::GameManager(sf::RenderWindow* _window) {
	window = _window;
	window->create(sf::VideoMode(WINDOW_PLAY_AREA_WIDTH + WINDOW_SIDEBAR_WIDTH, WINDOW_HEIGHT), "Breakout");
	texture.loadFromFile("textures.png");

	// Ball setup
	ball = Ball(7.f, sf::Vector2f(50.f, 500.f), sf::Vector2f(500.f, 500.f));
	ball.setTexture(&texture);
	ball.setTextureRect(sf::IntRect(75, 0, 50, 50));
	ball.setOrigin(7.f, 7.f);

	player.velocity = 50.f;
	/*
	player.setSize(sf::Vector2f(100, 15));
	player.setPosition(sf::Vector2f(245.f, 575.f));
	player.setFillColor(sf::Color(255, 255, 255));*/

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

void GameManager::movePlayer(int _direction) {
	BallDirection direction = BallDirection(_direction);
	if (direction == LEFT) {
		player.move(sf::Vector2f(player.velocity, 0.f));
	}
	else {
		player.move(sf::Vector2f(player.velocity, 0.f));
	}
}

void GameManager::drawQTshapes(QuadTree* qt) {
	window->draw(qt->bounds);
	if (qt->NW != nullptr) drawQTshapes(qt->NW);
	if (qt->NE != nullptr) drawQTshapes(qt->NE);
	if (qt->SW != nullptr) drawQTshapes(qt->SW);
	if (qt->SE != nullptr) drawQTshapes(qt->SE);
}

void GameManager::update(float deltaTime) {
	window->draw(background);	
	window->draw(player);

	for (Brick b : bricks) {
		if (b.hp != 0) {
			window->draw(b);
		}
	}
	//drawQTshapes(&qt);
	ballBrickCollision(qt.collisionBricks(ball));

	ball.rotate(90 * deltaTime);
	ball.Move(deltaTime, WINDOW_PLAY_AREA_WIDTH, WINDOW_HEIGHT);
	player.setPosition(sf::Vector2f((ball.getPosition().x - player.getSize().x / 2), player.getPosition().y));
	window->draw(ball);

	return;
}

void GameManager::ballBrickCollision(std::unordered_set<Brick*> collisionBricks) {
	if (checkCollision(&player, &ball)) {
		//std::cout << "Bounce off of player\n";
		ballBounce();
		return;
	}

	for (Brick* brick : collisionBricks) {
		if (brick->hp == 0) continue;

		if (checkCollision(brick, &ball)) {
			ballBounce();
			if (brick->hp == -1) {
				return;
			}
			if (--brick->hp == 0) {
				qt.deleteBrick(brick);
				return;
			}
			sf::IntRect brickTextureRect = brick->getTextureRect();
			brick->setTextureRect(sf::IntRect(
				brickTextureRect.left + Brick::BRICK_WIDTH,
				brickTextureRect.top,
				brickTextureRect.width,
				brickTextureRect.height
			));
		}
	}
}

void GameManager::ballBounce() {
	BallDirection direction = BallDirection(getDirection(ball.reposition));
	if (direction == LEFT || direction == RIGHT) {
		ball.velocity.x = -ball.velocity.x;
		if (direction == LEFT) {
			ball.move(sf::Vector2f(ball.getRadius() - std::abs(ball.reposition.x), 0.f));
		}
		else {
			ball.move(sf::Vector2f(-(ball.getRadius() - std::abs(ball.reposition.x)), 0.f));
		}
	}
	else {
		ball.velocity.y = -ball.velocity.y;
		if (direction == DOWN) {
			ball.move(sf::Vector2f(0.f, ball.getRadius() - std::abs(ball.reposition.y)));
		}
		else {
			ball.move(sf::Vector2f(0.f, -(ball.getRadius() - std::abs(ball.reposition.y))));
		}
	}
}

bool GameManager::checkCollision(RectangularObject* rect, Ball* ball) {
	float ballRadius = ball->getRadius();
	sf::Vector2f ballCenter = ball->getPosition();

	sf::Vector2f rectSize = rect->getSize();
	sf::Vector2f rectCenter = rect->getObjectCentre();

	sf::Vector2f circleCenterToBrick = (rectCenter + clamp(ballCenter - rectCenter, rectSize / 2.f)) - ballCenter;
	
	if (magnitude(circleCenterToBrick) <= ballRadius) {
		ball->reposition = circleCenterToBrick;
		return true;
	}
	return false;
}

// Direction order -> LEFT RIGHT UP DOWN
int GameManager::getDirection(sf::Vector2f vector) {
	sf::Vector2f directions[] = {
		sf::Vector2f(-1.f, 0.f),
		sf::Vector2f(1.f,  0.f),
		sf::Vector2f(0.f,  1.f),
		sf::Vector2f(0.f, -1.f)
	};

	float max = 0.0f;
	int bestMatch = -1;
	for (int i = 0; i < 4; i++) {
		float dotProd = dotProduct(vector, directions[i]);
		if (dotProd > max) {
			max = dotProd;
			bestMatch = i;
		}
	}
	return bestMatch;
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
				texturePositions.at(row[i]) * Brick::BRICK_HEIGHT,
				Brick::BRICK_WIDTH, 
				Brick::BRICK_HEIGHT));
			bricks.back().setPosition(sf::Vector2f(
				WINDOW_LEFT_OFFSET + i*(Brick::BRICK_WIDTH + levels[levelNumber].columnSpacing),
				WINDOW_TOP_OFFSET  + rowCount*(Brick::BRICK_HEIGHT + levels[levelNumber].rowSpacing)));
			window->draw(bricks.back());
		}
		rowCount++;
	}
}
