#pragma once

#include <string>

#include "Level.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class LevelGenerator {
	public:
		LevelGenerator();
		Level parseLevelInfo(int levelNumber = -1);
		std::vector<std::string> parseLevelLayout(std::string x);
		std::string getXMLStringAttribute(std::string attribute, XMLElement* element);

		const std::string LEVEL = "Level";
		const std::string BRICK_TYPES = "BrickTypes";
		const std::string BRICK_TYPE = "BrickType";
		const std::string BRICKS = "Bricks";
};