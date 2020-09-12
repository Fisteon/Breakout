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
	while (window.isOpen())	{
		while (window.pollEvent(event))		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (BreakoutGame.startGameIfReady()) {
					break;
				}
				if (event.key.code == sf::Keyboard::Q) {
					BreakoutGame.showQuadtree = !BreakoutGame.showQuadtree;
				}
				else if (event.key.code == sf::Keyboard::A) {
					BreakoutGame.autoplay = !BreakoutGame.autoplay;
				}
				else if (event.key.code == sf::Keyboard::Up) {
					BreakoutGame.nextLevel();
				}
				else if (event.key.code == sf::Keyboard::Down) {
					BreakoutGame.previousLevel();
				}
				else if (event.key.code == sf::Keyboard::P) {
					BreakoutGame.togglePlayState();
				}
				else if (event.key.code == sf::Keyboard::N) {
					BreakoutGame.newGame();
				}
				else if (event.key.code == sf::Keyboard::X) {
					window.close();
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			BreakoutGame.movePlayer(0, deltaTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			BreakoutGame.movePlayer(1, deltaTime.asSeconds());
		}

		window.clear();
		BreakoutGame.update(deltaTime.asSeconds());
		window.display();
		
		deltaTime = gameClock.restart();
	}

	return 0;
}