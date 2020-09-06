#pragma once

#include <vector>
#include <map>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include "Level.h"
#include "Ball.h"

class GameManager {
	public:
		GameManager(sf::RenderWindow* _window);

		void Update(float deltaTime);
		void initialLevelSetup(int levelNumber);

	private:
		sf::RenderWindow* window;
		sf::Texture texture;
		std::vector<Level> levels;
		std::vector<Brick> bricks;
		Ball ball;

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
