#pragma once

#include <string>

class Brick {
public:
	const char* texture;
	int hp;
	int breakScore;
	const char* hitSound;
	const char* breakSound;

	Brick();

	Brick(const char* _texture, const char* _hitSound, const char* _breakSound = "", int _hp = -1, int _breakScore = -1) :
		texture(_texture), 
		hp(_hp), 
		breakScore(_breakScore), 
		hitSound(_hitSound), 
		breakSound(_breakSound) {}

	~Brick();

};