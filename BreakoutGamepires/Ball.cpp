#include "Ball.h"

Ball::Ball() {

}

Ball::Ball(float _radius, sf::Vector2f _position, sf::Vector2f _velocity) {
	this->setRadius(_radius);
	this->setPosition(_position);
	velocity = _velocity;
}

void Ball::Move() {

}