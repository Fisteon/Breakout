#include "Brick.h"

Brick::Brick() {
	this->hp = -1;
	this->breakScore = -1;
}

Brick::Brick(sf::Vector2f _position, sf::Vector2f _size, int _textureX, int _textureY, int _hp, int _breakScore):
	textureX(_textureX),
	textureY(_textureY),
	hp(_hp),
	breakScore(_breakScore) {
	this->setPosition(_position);
	this->setSize(_size);

}

Brick::~Brick() {

}