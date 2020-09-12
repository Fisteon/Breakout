#pragma once

#include <tuple>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>
#include "Utility.h"

class Ball : public sf::CircleShape
{
public:
	Ball();
	Ball(float radius, sf::Vector2f position, sf::Vector2f velocity);


	bool travel(float deltaTime, int windowWidth, int windowHeight);
	sf::Vector2f velocity;
	sf::Vector2f reposition;
};