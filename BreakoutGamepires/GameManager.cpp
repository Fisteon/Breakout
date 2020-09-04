#include "GameManager.h"
#include "LevelGenerator.h"

GameManager::GameManager() {
	//sf::RenderWindow window(sf::VideoMode(width, height), "Breakout");
	LevelGenerator lg;
	Level x = lg.parseLevelInfo(1);

}