#include "GameManager.h"
#include "LevelGenerator.h"

GameManager::GameManager(int width, int height) {
	sf::RenderWindow window(sf::VideoMode(width, height), "Breakout");
	LevelGenerator lg(1);

}