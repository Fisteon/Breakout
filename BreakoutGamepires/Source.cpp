#include <iostream>
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "GameManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(610, 600), "SFML works!");
	sf::RectangleShape blocks[30];

	GameManager g;

	Ball b;
	b.direction.x = 0.1f;
	b.direction.y = 0.1f;

	for (int i = 0; i < 30; i++) {
		blocks[i].setSize(sf::Vector2f(50.f, 25.f));
		blocks[i].setOutlineColor(sf::Color(255, 0, 0));
		blocks[i].setOutlineThickness(3.f);
		blocks[i].setFillColor(sf::Color(0, 0, 0, 0));
	}

	sf::RectangleShape pad(sf::Vector2f(100.f, 20.f));
	pad.setPosition(sf::Vector2f(255.f, 550.f));

	sf::CircleShape shape(7.f);
	shape.setPosition(sf::Vector2f(305, 300));
	shape.setFillColor(sf::Color::Green);

	int time = 0;

	while (window.isOpen())
	{
		time++;
		shape.move(b.direction);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//window.draw(shape);
		int count = 1;
		int pos_X = 10;
		int pos_Y = 10;
		for each (sf::RectangleShape block in blocks)
		{
			block.setPosition(sf::Vector2f(pos_X, pos_Y));
			pos_X += 60;
			if (count % 10 == 0) {
				pos_Y += 35;
				pos_X = 10;
			}
			count++;
			window.draw(block);
		}
		window.draw(pad);
		window.draw(shape);

		window.display();
	}

	return 0;
}