#pragma once
#include <tuple>
#include "Utility.h"
#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>


class Ball : public sf::CircleShape
{
public:
	Ball();
	Ball(float radius, sf::Vector2f position, sf::Vector2f velocity);


	bool Move(float deltaTime, int windowWidth, int windowHeight);
	sf::Vector2f velocity;
	sf::Vector2f reposition;
};