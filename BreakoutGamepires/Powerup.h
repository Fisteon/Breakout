#pragma once

#include <string>
#include <stdlib.h>

#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>

class Powerup : public sf::RectangleShape {
public:
	Powerup();
	~Powerup();

	 enum powerupType {
		FIREBALL,
		EXTENSION
	};

	powerupType type;

	bool active;
	float duration;
	float fallingSpeed = 250.f;
private:
	int textureX;
	int textureY;
};
