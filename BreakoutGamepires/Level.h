#pragma once

#include <string>
#include <map>
#include <vector>

#include "Brick.h"

class Level
{
public:


	Level();

	Level(int _rowCount, int _columnCount, int _rowSpacing, int _columnSpacing, const char* _backgroundTexture, const char* _spriteTexture, std::map<char, Brick> _bricks) :
		rowCount(_rowCount),
		columnCount(_columnCount),
		rowSpacing(_rowSpacing),
		columnSpacing(_columnSpacing),
		backgroundTexture(_backgroundTexture),
		spriteTexture(_spriteTexture),
		brickTypes(_bricks) {}
	~Level();

	int rowCount;
	int columnCount;
	int rowSpacing;
	int columnSpacing;
	
	std::string spriteTexture;
	std::string backgroundTexture;
	std::map<char, Brick> brickTypes;
	std::vector<std::string> levelLayout;
};

