#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "GameManager.h"


int main()
{
	sf::RenderWindow window;
	GameManager BreakoutGame(&window);
	
	sf::Clock gameClock;
	sf::Time deltaTime;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		BreakoutGame.Update(deltaTime.asSeconds());
		window.display();
		
		deltaTime = gameClock.restart();
	}

	return 0;
}