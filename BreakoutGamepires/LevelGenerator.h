#pragma once

#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

class LevelGenerator {
public:
	LevelGenerator(int levelNumber = -1);

	const std::string LEVEL = "Level";
	const std::string BRICK_TYPES = "BrickTypes";
	const std::string BRICK_TYPE = "BrickType";
	const std::string BRICKS = "Bricks";
};