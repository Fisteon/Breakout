#pragma once

#include <string>
#include <SFML\Main.hpp>
#include <SFML\Graphics.hpp>

class Brick : public sf::RectangleShape {
public:
	int textureX;
	int textureY;
	int texturesLeft;
	int hp;
	int breakScore;
	
	std::string hitSound;
	std::string breakSound;

	Brick();
	Brick(
		sf::Vector2f _position, 
		sf::Vector2f _size, 
		int _textureX, 
		int _textureY,
		const char* _hitSound, 
		const char* _breakSound = "", 
		int _hp = -1, 
		int _breakScore = -1
	);

	~Brick();

	static int const BRICK_WIDTH	= 50;
	static int const BRICK_HEIGHT	= 30;
};