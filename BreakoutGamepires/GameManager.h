#pragma once

#include <vector>
#include <unordered_set>
#include <iostream>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Ball.h"
#include "Level.h"
#include "Powerup.h"
#include "QuadTree.h"
#include "UI.h"

class GameManager {
	public:
		GameManager(sf::RenderWindow* _window);

		void initialize();
		void reset();
		void newGame();
		void loadSound(sf::SoundBuffer* buffer, sf::Sound* sound, std::string path);
		void checkLevelCompletion();

		void nextLevel();
		void previousLevel();

		bool startGameIfReady();
		void update(float deltaTime);
		void drawUI();
		void drawQTshapes(QuadTree* qt);
		
		void movePlayer(int direction, float deltaTime);

		void spawnPowerup();
		void activatePowerup(Powerup powerup);
		void deactivatePowerup(Powerup::powerupType type);
		bool isPowerupActive(Powerup::powerupType type);
		void managePowerups(float deltaTime);

		void ballBrickCollision(std::unordered_set<Brick*> collisionBricks);
		template <typename T>
		bool checkCollision(T* rect, Ball* ball);
		bool checkCollision(sf::RectangleShape rectA, sf::RectangleShape rectB);
		void ballBounce();
		void adaptBallVelocity();
		void initialLevelSetup(int levelNumber);
		void togglePlayState();

		bool showQuadtree = false;
		bool autoplay = false;

		static const int WINDOW_SIDEBAR_WIDTH	= 275;
		static const int WINDOW_PLAY_AREA_WIDTH = 1040;
		static const int WINDOW_HEIGHT			= 1080;

	private:
		sf::RenderWindow* window;

		sf::SoundBuffer bufferBallFail;
		sf::SoundBuffer bufferBallBounce;
		sf::SoundBuffer bufferBrickHit;
		sf::SoundBuffer bufferBrickBreak;
		sf::SoundBuffer bufferImpenetrableHit;
		sf::Sound		soundImpenetrableHit;
		sf::Sound		soundBallFail;
		sf::Sound		soundBallBounce;
		sf::Sound		soundBrickHit;
		sf::Sound		soundBrickBreak;

		sf::Texture texture;
		std::vector<Level> levels;
		std::vector<Brick> bricks;
		std::vector<Powerup> powerups;
		std::vector<Powerup> activePowerups;
		
		Ball ball;
		sf::RectangleShape player;

		QuadTree qt;

		UI ui;

		int lives = 4;
		int score = 0;
		int currentLevel = 0;

		float playerVelocity;
		float ballRadius = 7.f;
		float timeCounter = 0.f;

		const int WINDOW_LEFT_OFFSET		= 10;
		const int WINDOW_TOP_OFFSET			= 10;
		const float INITIAL_PLAYER_SPEED	= 750.f;
		const float INITIAL_BALL_SPEED_X	= 750.f;
		const float INITIAL_BALL_SPEED_Y	= -750.f;

		enum GameState {
			LOADING,
			READY,
			PLAY,
			PAUSE,
			NEWBALL,
			LEVEL_COMPLETE,
			WIN,
			LOSE
		};
		GameState gameState;

		const enum BallDirection {
			LEFT,
			RIGHT,
			UP,
			DOWN
		};
};
