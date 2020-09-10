#pragma once

#include <string>
#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>

class RectangularObject : public sf::RectangleShape {
public:
	/*const char**/ std::string texture;
	/*const char**/ std::string hitSound;

	sf::Vector2f getObjectCentre() {
		return getPosition() + (getSize() / 2.f);
	}

	float velocity;
};