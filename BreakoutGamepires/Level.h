#pragma once

#include <map>
#include <vector>

#include "Brick.h"

class Level
{
public:


	Level();

	Level(int _rowCount, int _columnCount, int _rowSpacing, int _columnSpacing, const char* _backgroundTexture,
		std::map<const char*, Brick> _bricks) :
		rowCount(_rowCount),
		columnCount(_columnCount),
		rowSpacing(_rowSpacing),
		columnSpacing(_columnSpacing),
		backgroundTexture(_backgroundTexture),
		brickTypes(_bricks) {}
	~Level();

	int rowCount;
	int columnCount;
	int rowSpacing;
	int columnSpacing;
	const char* backgroundTexture;
	std::map<const char*, Brick> brickTypes;
	std::vector<std::string> levelLayout;
};

