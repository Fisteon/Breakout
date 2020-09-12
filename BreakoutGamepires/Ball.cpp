#include "Ball.h"

Ball::Ball() {

}

Ball::Ball(float _radius, sf::Vector2f _position, sf::Vector2f _velocity) {
	this->setRadius(_radius);
	this->setPosition(_position);
	velocity = _velocity;
}

bool Ball::travel(float deltaTime, int windowWidth, int windowHeight) {
	move(velocity * deltaTime);
	rotate(90 * deltaTime);
	// Outside left window border
	if (getPosition().x - getRadius() <= 0.f) {
		velocity.x = -velocity.x;
		setPosition(0.f + getRadius(), getPosition().y);
		return true;
	} // Outside right window border
	else if (getPosition().x + getRadius() >= windowWidth) {
		velocity.x = -velocity.x;
		setPosition(windowWidth - getRadius(), getPosition().y);
		return true;
	}
	// Outside top window border
	if (getPosition().y - getRadius() <= 0.f) {
		velocity.y = -velocity.y;
		setPosition(getPosition().x, 0.f + getRadius());
		return true;
	}
	else if (getPosition().y + getRadius() >= windowHeight) {
		//velocity.y = -velocity.y;
		//setPosition(getPosition().x, windowHeight - getRadius());
		return false;
	}
	
}