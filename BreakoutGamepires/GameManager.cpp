#include "UI.h";
#include "Utility.h"
#include "GameManager.h"
#include "LevelGenerator.h"
#include "QuadTree.h"


GameManager::GameManager(sf::RenderWindow* _window) {
	window = _window;
	window->create(
		sf::VideoMode(
			WINDOW_PLAY_AREA_WIDTH + WINDOW_SIDEBAR_WIDTH, 
			WINDOW_HEIGHT),
		"Breakout", 
		sf::Style::Titlebar | sf::Style::Close
	);

	bufferBallFail.loadFromFile("ball_miss.wav");
	bufferBallBounce.loadFromFile("ball_bounce.wav");
	bufferBrickHit.loadFromFile("hit_sound.wav");
	bufferBrickBreak.loadFromFile("break_sound.wav");
	bufferImpenetrableHit.loadFromFile("impenetrable_hit.wav");
	soundBallFail.setBuffer(bufferBallFail);
	soundBallBounce.setBuffer(bufferBallBounce);
	soundBrickHit.setBuffer(bufferBrickHit);
	soundBrickBreak.setBuffer(bufferBrickBreak);
	soundImpenetrableHit.setBuffer(bufferImpenetrableHit);

	// Level setup
	LevelGenerator levelGenerator;
	int levelNumber = 1;
	while (fileExists("LevelData_" + std::to_string(levelNumber) + ".xml")) {
		levels.push_back(levelGenerator.parseLevelInfo(levelNumber));
		levelNumber++;
	}
	
	texture.loadFromFile(levels[currentLevel].spriteTexture);
	ui.backgroundTexture.loadFromFile(levels[currentLevel].backgroundTexture);

	// Ball setup
	ball = Ball(ballRadius, sf::Vector2f(50.f, 500.f), sf::Vector2f(INITIAL_BALL_SPEED_X, INITIAL_BALL_SPEED_Y));
	ball.setTexture(&texture);
	ball.setTextureRect(sf::IntRect(150, 0, 100, 100));
	ball.setOrigin(ballRadius, ballRadius);

	// Player setup
	player.setTexture(&texture);
	player.setTextureRect(sf::IntRect(100, 104, 64, 16));
	player.setSize(sf::Vector2f(100.f, 15.f));
	playerVelocity = 2.f;

	// Final UI setup
	ui.hearts.setTexture(&texture);
	ui.background.setTexture(&ui.backgroundTexture);
	ui.repositionStatusText();

	initialize();
	
	return;
}

void GameManager::initialize() {
	ball.velocity = sf::Vector2f(INITIAL_BALL_SPEED_X, INITIAL_BALL_SPEED_Y);
	ball.setPosition(WINDOW_PLAY_AREA_WIDTH / 2, WINDOW_HEIGHT / 2);
	player.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH / 2 - player.getSize().x / 2, WINDOW_HEIGHT - 25));
	ui.level.setString("LEVEL " + std::to_string(currentLevel + 1));
	ui.status.setString("Press any key to start!");
	ui.repositionStatusText();

	// Bricks setup
	initialLevelSetup(currentLevel);

	// QuadTree setup
	qt = QuadTree(0.f, 0.f, WINDOW_PLAY_AREA_WIDTH, WINDOW_HEIGHT, 0);
	for (int i = 0; i < bricks.size(); i++) {
		qt.addBrick(&bricks[i]);
	}

	gameState = READY;
}

void GameManager::reset() {
	currentLevel = 0;
	lives = 4;
	score = 0;
}

void GameManager::newGame() {
	gameState = READY;
	reset();
	initialize();
}

bool GameManager::startGameIfReady() {
	if (gameState == READY) {
		gameState = PLAY;
		return true;
	}
	return false;
}

void GameManager::update(float deltaTime) {
	window->draw(ui.background);
	window->draw(player);

	for (Brick b : bricks) {
		if (b.hp != 0) {
			window->draw(b);
		}
	}

	window->draw(ball);
	drawUI();

	if (gameState == LOSE) {
		window->draw(ui.status);
		if (timeCounter >= 5.f) {
			timeCounter = 0;
			reset();
			initialize();
		}
		else {
			timeCounter += deltaTime;
		}
	}

	if (gameState == NEWBALL) {
		if (timeCounter >= 2.f) {
			player.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH / 2 - player.getSize().x / 2, WINDOW_HEIGHT - 25));
			ball.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH / 2, WINDOW_HEIGHT - 25 - ballRadius));
			ball.velocity = sf::Vector2f(INITIAL_BALL_SPEED_X, INITIAL_BALL_SPEED_Y);
			timeCounter = 0;
			gameState = READY;
		}
		else {
			timeCounter += deltaTime;
		}
	}

	if (gameState == PAUSE || gameState == READY) {
		window->draw(ui.status);
		return;
	}
	if (gameState == PLAY) {
		ballBrickCollision(qt.collisionBricks(ball));

		if (!ball.Move(deltaTime, WINDOW_PLAY_AREA_WIDTH, WINDOW_HEIGHT)) {
			lives--;
			soundBallFail.play();
			ball.move(sf::Vector2f(0, 50));
			if (lives == 0) {
				gameState = LOSE;
				ui.status.setString("Game over!");
				ui.repositionStatusText();
			}
			else {
				gameState = NEWBALL;
			}			
		}
		if (autoplay) {
			player.setPosition(sf::Vector2f((ball.getPosition().x - player.getSize().x / 2), player.getPosition().y));
		}
	}

	if (showQuadtree) {
		drawQTshapes(&qt);
	}
}

void GameManager::drawUI() {
	ui.score.setString("SCORE : " + std::to_string(score));
	for (int i = 0; i < lives; i++) {
		ui.hearts.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH + 25 + 30 * i, 50));
		window->draw(ui.hearts);
	}

	window->draw(ui.level);
	window->draw(ui.score);
	window->draw(ui.pause);
	window->draw(ui.newgame);
	window->draw(ui.exit);
	window->draw(ui.quadtree);
	window->draw(ui.autoplay);

	window->draw(ui.UIborder);
}

void GameManager::drawQTshapes(QuadTree* qt) {
	window->draw(qt->bounds);
	if (qt->NW != nullptr) drawQTshapes(qt->NW);
	if (qt->NE != nullptr) drawQTshapes(qt->NE);
	if (qt->SW != nullptr) drawQTshapes(qt->SW);
	if (qt->SE != nullptr) drawQTshapes(qt->SE);
}

void GameManager::movePlayer(int _direction) {
	BallDirection direction = BallDirection(_direction);
	if (direction == LEFT) {
		player.move(sf::Vector2f(-playerVelocity, 0.f));
		if (player.getPosition().x < 0) {
			player.setPosition(sf::Vector2f(0, player.getPosition().y));
		}
	}
	else {
		player.move(sf::Vector2f(playerVelocity, 0.f));
		if (player.getPosition().x + player.getSize().x > WINDOW_PLAY_AREA_WIDTH) {
			player.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH - player.getSize().x, player.getPosition().y));
		}
	}
}

// Processes collisions with bricks provided by QuadTree
void GameManager::ballBrickCollision(std::unordered_set<Brick*> collisionBricks) {
	if (checkCollision(&player, &ball)) {
		ballBounce();
		adaptBallVelocity();
		soundBallBounce.play();
		return;
	}

	for (Brick* brick : collisionBricks) {
		if (brick->hp == 0) continue;

		if (checkCollision(brick, &ball)) {
			ballBounce();
			if (brick->hp == -1) {
				soundImpenetrableHit.play();
				return;
			}
			if (--brick->hp == 0) {
				qt.deleteBrick(brick);
				score += brick->breakScore;
				soundBrickBreak.play();
				return;
			}
			if (brick->texturesLeft > 0) {
				sf::IntRect brickTextureRect = brick->getTextureRect();
				brick->setTextureRect(sf::IntRect(
					brickTextureRect.left + Brick::BRICK_WIDTH,
					brickTextureRect.top,
					brickTextureRect.width,
					brickTextureRect.height
				));
				brick->texturesLeft--;
			}
			soundBrickHit.play();
		}
	}
}

// Checks for collision between the ball and a brick/paddle
template <typename T>
bool GameManager::checkCollision(T* rect, Ball* ball) {
	float ballRadius = ball->getRadius();
	sf::Vector2f ballCenter = ball->getPosition();

	sf::Vector2f rectSize = rect->getSize();
	sf::Vector2f rectCenter = rect->getPosition() + (rect->getSize() / 2.f);

	sf::Vector2f circleCenterToBrick = (rectCenter + clamp(ballCenter - rectCenter, rectSize / 2.f)) - ballCenter;

	if (magnitude(circleCenterToBrick) <= ballRadius) {
		ball->reposition = circleCenterToBrick;
		return true;
	}
	return false;
}

// Changes ball's movement direction on collision with an object
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

int GameManager::getDirection(sf::Vector2f vector) {
	// Direction order -> LEFT RIGHT UP DOWN
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

// Changes ball's velocity depending on the point of impact on the paddle
void GameManager::adaptBallVelocity() {
	float percentage = 
		(ball.getPosition().x - (player.getPosition().x + player.getSize().x / 2.f)) /
		(player.getSize().x / 2.f);;
	percentage = clamp(percentage, -0.5f, 0.5f);
	ball.velocity.x += INITIAL_BALL_SPEED_X * percentage;
	ball.velocity.x = clamp(ball.velocity.x, -INITIAL_BALL_SPEED_X, INITIAL_BALL_SPEED_X);
}

// Sets up all bricks as read from the level XML
void GameManager::initialLevelSetup(int levelNumber) {
	bricks.clear();
	int rowCount = 0;
	for (std::string row : levels[levelNumber].levelLayout) {
		for (int i = 0; i < row.length(); i++) {
			if (row[i] == '_') continue;
			bricks.push_back(levels[levelNumber].brickTypes[row[i]]);
			bricks.back().setSize(sf::Vector2f(Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT));
			bricks.back().setTexture(&texture);
			bricks.back().setTextureRect(sf::IntRect(
				bricks.back().textureX, 
				bricks.back().textureY,//texturePositions.at(row[i]) * Brick::BRICK_HEIGHT,
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

void GameManager::togglePlayState() {
	if (gameState == PAUSE) {
		gameState = PLAY;
		return;
	}
	if (gameState == PLAY) {
		gameState = PAUSE;
		ui.status.setString("Press P to resume!");
		ui.repositionStatusText();
		return;
	}
}
