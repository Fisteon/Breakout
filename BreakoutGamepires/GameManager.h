#pragma once

#include <vector>
#include <map>
#include <iostream>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include "Level.h"
#include "Ball.h"

#include "QuadTree.h"

class GameManager {
	public:
		GameManager(sf::RenderWindow* _window);

		void drawQTshapes(QuadTree* qt);

		void update(float deltaTime);
		void initialLevelSetup(int levelNumber);
		void deleteBrick(int i);

	private:
		sf::RenderWindow* window;
		sf::Texture texture;
		std::vector<Level> levels;
		std::vector<Brick> bricks;
		Ball ball;

		QuadTree qt;

		sf::RectangleShape background;

		const int WINDOW_SIDEBAR_WIDTH		= 75;
		const int WINDOW_PLAY_AREA_WIDTH	= 540;
		const int WINDOW_HEIGHT				= 600;
		const std::map<char, int> texturePositions{
			{ 'H', 0},
			{ 'M', 1 },
			{ 'S', 2 },
			{ 'I', 3 }
		};
};
