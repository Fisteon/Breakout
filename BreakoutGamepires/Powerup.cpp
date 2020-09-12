#include "Powerup.h"

Powerup::Powerup() {
	srand(time(NULL));
	active = false;

	int random = rand() % 100 + 1;
	if (random > 65) {
		type = FIREBALL;
		duration = 5.f;
		setSize(sf::Vector2f(50, 50));
		setTextureRect(sf::IntRect(100, 30, 50, 50));
	}
	else {
		type = EXTENSION;
		duration = 15.f;
		setSize(sf::Vector2f(50, 21));
		setTextureRect(sf::IntRect(150, 30, 50, 21));
	}
}

Powerup::~Powerup() {

}