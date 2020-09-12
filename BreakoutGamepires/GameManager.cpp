#include "GameManager.h"
#include "LevelGenerator.h"

GameManager::GameManager(sf::RenderWindow* _window) {
	window = _window;
	window->create(
		sf::VideoMode(
			WINDOW_PLAY_AREA_WIDTH + WINDOW_SIDEBAR_WIDTH, 
			WINDOW_HEIGHT),
		"Breakout", 
		sf::Style::Titlebar | sf::Style::Close
	);
	srand(time(NULL));
	
	// Level setup
	LevelGenerator levelGenerator;
	int levelNumber = 1;
	while (fileExists("Resources/Levels/LevelData_" + std::to_string(levelNumber) + ".xml")) {
		levels.push_back(levelGenerator.parseLevelInfo(levelNumber));
		levelNumber++;
	}

	// Sound setup
	loadSound(&bufferBallFail, &soundBallFail, "BallMiss");
	loadSound(&bufferBallBounce, &soundBallBounce, "BallBounce");
	loadSound(&bufferBrickHit, &soundBrickHit, "Hit");
	loadSound(&bufferBrickBreak, &soundBrickBreak, "Break");
	loadSound(&bufferImpenetrableHit, &soundImpenetrableHit, "InfiniteHit");
	
	// Textures setup
	texture.loadFromFile(levels[currentLevel].spriteTexture);
	ui.backgroundTexture.loadFromFile(levels[currentLevel].backgroundTexture);

	// Ball setup
	ball = Ball(ballRadius, sf::Vector2f(50.f, 500.f), sf::Vector2f(INITIAL_BALL_SPEED_X, INITIAL_BALL_SPEED_Y));
	ball.setTexture(&texture);
	ball.setTextureRect(sf::IntRect(150, 0, 29, 29));
	ball.setOrigin(ballRadius, ballRadius);

	// Player setup
	player.setTexture(&texture);
	player.setTextureRect(sf::IntRect(100, 104, 64, 16));
	player.setSize(sf::Vector2f(100.f, 15.f));
	playerVelocity = INITIAL_PLAYER_SPEED;

	// Final UI setup
	ui.hearts.setTexture(&texture);
	ui.background.setTexture(&ui.backgroundTexture);
	ui.repositionStatusText();

	initialize();
	
	return;
}

void GameManager::initialize() {
	powerups.clear();
	for (Powerup &p : activePowerups) deactivatePowerup(p.type);
	activePowerups.clear();
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
	//gameState = READY;
	reset();
	initialize();
}

void GameManager::loadSound(sf::SoundBuffer* buffer, sf::Sound* sound, std::string path) {
	if (buffer->loadFromFile(levels[currentLevel].soundPaths[path])) {
		sound->setBuffer(*buffer);
	}
}

void GameManager::checkLevelCompletion() {
	for (int i = 0; i < bricks.size(); i++) {
		if (bricks[i].hp > 0) {
			return;
		}
	}
	gameState = LEVEL_COMPLETE;
}

void GameManager::nextLevel() {
	gameState = LEVEL_COMPLETE;
}

void GameManager::previousLevel() {
	if (currentLevel > 0) {
		currentLevel--;
		initialize();
	}
}

bool GameManager::startGameIfReady() {
	if (gameState == LOADING) {
		initialize();
		gameState = READY;
		return true;
	}
	if (gameState == READY) {
		gameState = PLAY;
		return true;
	}
	if (gameState == WIN) {
		newGame();
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

	managePowerups(deltaTime);
	window->draw(ball);
	drawUI();

	if (gameState == LEVEL_COMPLETE) {
		if (currentLevel < levels.size() - 1) {
			currentLevel++;
			ui.status.setString("Good job!\nPress any key for the next level...");
			ui.repositionStatusText();
			gameState = LOADING;
		}
		else {
			ui.status.setString("Congratulations! You win!\nYour score: " + std::to_string(score) + "\nPress any key to play again.");
			ui.repositionStatusText();
			gameState = WIN;
		}
		return;
	}

	if (gameState == WIN) {
		window->draw(ui.status);
		return;
	}

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
		return;
	}

	if (gameState == NEWBALL) {
		if (timeCounter >= 2.f) {
			for (Powerup &p : activePowerups) deactivatePowerup(p.type);
			powerups.clear();
			activePowerups.clear();
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

	if (gameState == PAUSE || gameState == READY || gameState == LOADING) {
		window->draw(ui.status);
		return;
	}
	if (gameState == PLAY) {
		ballBrickCollision(qt.collisionBricks(ball));

		if (!ball.travel(deltaTime, WINDOW_PLAY_AREA_WIDTH, WINDOW_HEIGHT)) {
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

	checkLevelCompletion();

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

// Draws QuadTree grid over the play area
void GameManager::drawQTshapes(QuadTree* qt) {
	window->draw(qt->bounds);
	if (qt->isSplit()) {
		drawQTshapes(qt->NW);
		drawQTshapes(qt->NE);
		drawQTshapes(qt->SW);
		drawQTshapes(qt->SE);
	}
}

void GameManager::movePlayer(int _direction, float deltaTime) {
	BallDirection direction = BallDirection(_direction);
	if (direction == LEFT) {
		player.move(sf::Vector2f(-playerVelocity * deltaTime, 0.f));
		if (player.getPosition().x < 0) {
			player.setPosition(sf::Vector2f(0, player.getPosition().y));
		}
	}
	else {
		player.move(sf::Vector2f(playerVelocity * deltaTime, 0.f));
		if (player.getPosition().x + player.getSize().x > WINDOW_PLAY_AREA_WIDTH) {
			player.setPosition(sf::Vector2f(WINDOW_PLAY_AREA_WIDTH - player.getSize().x, player.getPosition().y));
		}
	}
}

void GameManager::spawnPowerup() {
	int x = rand() % 100 + 1;
	if (x > 90) {
		powerups.push_back(Powerup());
		powerups.back().setTexture(&texture);
		powerups.back().setPosition(ball.getPosition());
	}
}

bool GameManager::isPowerupActive(Powerup::powerupType type) {
	for (Powerup p : activePowerups) {
		if (p.type == type) {
			return true;
		}
	}
	return false;
}

// Activates the effect of a given powerup and changes visual attributes
void GameManager::activatePowerup(Powerup powerup) {
	if (isPowerupActive(powerup.type)) {
		for (Powerup &activePowerup : activePowerups) {
			if (activePowerup.type == powerup.type) {
				activePowerup.duration = powerup.duration;
			}
		}
	}
	else {
		activePowerups.push_back(Powerup(powerup));
		activePowerups.back().setPosition(sf::Vector2f(
			WINDOW_PLAY_AREA_WIDTH + 25 + 75 * activePowerups.back().type,
			500.f
		));
		if (activePowerups.back().type == Powerup::powerupType::FIREBALL) {
			ball.setFillColor(sf::Color(255, 125, 125));
		}
		if (activePowerups.back().type == Powerup::powerupType::EXTENSION) {
			player.setSize(sf::Vector2f(player.getSize().x *1.5f, player.getSize().y));
		}
	}
}

// Removes powerup from collection of active powerups and 
// performs required deactivation operations
void GameManager::deactivatePowerup(Powerup::powerupType type) {
	if (type == Powerup::powerupType::FIREBALL) {
		ball.setFillColor(sf::Color(255, 255, 255));
	}
	else if (type == Powerup::powerupType::EXTENSION) {
		player.setSize(sf::Vector2f(player.getSize().x / 1.5f, player.getSize().y));
	}
}

// Controls complete lifecycle of uncollected powerups and 
// lifetime of collected powerups
void GameManager::managePowerups(float deltaTime) {
	for (Powerup &powerup : powerups) {
		powerup.move(sf::Vector2f(0.f, powerup.fallingSpeed * deltaTime));
		if (checkCollision(powerup, player)) {
			activatePowerup(powerup);
			powerup.active = true;
		}
		window->draw(powerup);
	}
	
	for (Powerup &activePowerup : activePowerups) {
		window->draw(activePowerup);
		if (gameState == PAUSE) return;
		if (activePowerup.duration >= 2.f && activePowerup.duration - deltaTime < 2.f) {
			activePowerup.setFillColor(sf::Color(255, 255, 255, 128));
		}
		activePowerup.duration -= deltaTime;
		if (activePowerup.duration <= 0) {
			deactivatePowerup(activePowerup.type);
		}
	}

	powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [](const Powerup p) {
		return p.active || p.getPosition().y >= WINDOW_HEIGHT; }
	), powerups.end());

	activePowerups.erase(std::remove_if(activePowerups.begin(), activePowerups.end(), [](const Powerup p) {
		return p.duration <= 0.f; }
	), activePowerups.end());
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
			if (brick->hp == -1) {
				soundImpenetrableHit.play();
				ballBounce();
				return;
			}
			if (!isPowerupActive(Powerup::powerupType::FIREBALL)) {
				ballBounce();
			}
			if (--brick->hp == 0) {
				qt.deleteBrick(brick);
				score += brick->breakScore;
				soundBrickBreak.play();
				spawnPowerup();
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

// Checks for any AABB - AABB collision
bool GameManager::checkCollision(sf::RectangleShape rectA, sf::RectangleShape rectB) {
	bool X = rectA.getPosition().x - rectA.getSize().x / 2.f < rectB.getPosition().x + rectB.getSize().x &&
		rectA.getPosition().x + rectA.getSize().x > rectB.getPosition().x;
	bool Y = rectA.getPosition().y - rectA.getSize().y / 2.f < rectB.getPosition().y + rectB.getSize().y &&
		rectA.getPosition().y + rectA.getSize().y > rectB.getPosition().y;
	return X && Y;
}

// Changes ball's movement direction on collision with an object
void GameManager::ballBounce() {
	BallDirection direction = BallDirection(getVectorDirection(ball.reposition));
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
			if ((i + 1) * (Brick::BRICK_WIDTH + levels[levelNumber].columnSpacing) > (WINDOW_PLAY_AREA_WIDTH - WINDOW_LEFT_OFFSET)) continue;
			bricks.push_back(levels[levelNumber].brickTypes[row[i]]);
			bricks.back().setSize(sf::Vector2f(Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT));
			bricks.back().setTexture(&texture);
			bricks.back().setTextureRect(sf::IntRect(
				bricks.back().textureX, 
				bricks.back().textureY,
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
