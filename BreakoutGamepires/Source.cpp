#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "GameManager.h"


int main()
{
	sf::RenderWindow window;
	sf::Event event;
	GameManager BreakoutGame(&window);
	
	sf::Clock gameClock;
	sf::Time deltaTime;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			/*
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Left) {
					BreakoutGame.movePlayer(0);
				}
				else if (event.key.code == sf::Keyboard::Right) {
					BreakoutGame.movePlayer(1);
				}
			}*/
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			BreakoutGame.movePlayer(0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			BreakoutGame.movePlayer(1);
		}

		window.clear();
		BreakoutGame.update(deltaTime.asSeconds());
		window.display();
		
		deltaTime = gameClock.restart();
	}

	return 0;
}