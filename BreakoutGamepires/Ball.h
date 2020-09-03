#pragma once
#include <tuple>
#include "Utility.h"
#include "SFML\Main.hpp"


class Ball
{
public:
	float getVelocity() { return velocity; }
	void setVelocity(float v) {	velocity = v; }

	float getSize() { return size; }
	void setSize(float s) {	size = s; }

	sf::Vector2f direction;

private:
	float velocity;
	float size;
};