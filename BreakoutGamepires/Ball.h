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
	void Move();
	sf::Vector2f velocity;
};