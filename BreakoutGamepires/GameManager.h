#pragma once

#include <vector>
#include <unordered_set>
#include <map>
#include <iostream>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include "RectangularObject.h"
#include "Level.h"
#include "Ball.h"

#include "QuadTree.h"

class GameManager {
	public:
		GameManager(sf::RenderWindow* _window);

		void drawQTshapes(QuadTree* qt);

		void update(float deltaTime);
		void initialLevelSetup(int levelNumber);
		//void deleteBrick(Brick* brick);
		void ballBrickCollision(std::unordered_set<Brick*> collisionBricks);
		bool checkCollision(RectangularObject* brick, Ball* ball);
		bool checkcol(RectangularObject* brick, Ball* ball);
		void ballBounce();
		int getDirection(sf::Vector2f vector);

		void movePlayer(int direction);

	private:
		sf::RenderWindow* window;
		sf::Texture texture;
		std::vector<Level> levels;
		std::vector<Brick> bricks;
		Ball ball;
		//RectangularObject player;

		QuadTree qt;

		sf::RectangleShape background;

		const int WINDOW_SIDEBAR_WIDTH		= 275;
		const int WINDOW_PLAY_AREA_WIDTH	= 540;
		const int WINDOW_HEIGHT				= 600;
		const int WINDOW_LEFT_OFFSET		= 10;
		const int WINDOW_TOP_OFFSET			= 10;
		const std::map<char, int> texturePositions{
			{ 'H', 0},
			{ 'M', 1 },
			{ 'S', 2 },
			{ 'I', 3 }
		};

		const enum BallDirection {
			LEFT,
			RIGHT,
			UP,
			DOWN
		};
};
