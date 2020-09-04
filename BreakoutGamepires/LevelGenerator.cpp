#include <iostream>

#include "Utility.h"
#include "LevelGenerator.h"

LevelGenerator::LevelGenerator() {

}

Level LevelGenerator::parseLevelInfo(int levelNumber) {
	Level level;

	XMLDocument levelInfo;

	levelInfo.LoadFile(("LevelData_" + std::to_string(levelNumber) + ".xml").c_str());
	XMLNode * docRoot = levelInfo.FirstChild();

	docRoot->ToElement()->QueryIntAttribute("RowCount", &level.rowCount);
	docRoot->ToElement()->QueryIntAttribute("ColumnCount", &level.columnCount);
	docRoot->ToElement()->QueryIntAttribute("RowSpacing", &level.rowSpacing);
	docRoot->ToElement()->QueryIntAttribute("ColumnSpacing", &level.columnSpacing);
	docRoot->ToElement()->QueryStringAttribute("BackgroundTexture", &level.backgroundTexture);

	XMLElement* bricktypes = docRoot->FirstChildElement("BrickTypes");

	for (XMLElement* b = bricktypes->FirstChildElement(); b != NULL; b = b->NextSiblingElement()) {
		Brick brick;
		const char* brickID;
		b->QueryStringAttribute("Id", &brickID);
		b->QueryStringAttribute("Texture", &brick.texture);
		b->QueryIntAttribute("HitPoints", &brick.hp);
		b->QueryStringAttribute("HitSound", &brick.hitSound);
		b->QueryStringAttribute("BreakSound", &brick.breakSound);
		b->QueryIntAttribute("BreakScore", &brick.breakScore);

		level.brickTypes[brickID] = brick;
	}

	level.levelLayout = parseLevelLayout(removeSpaces(docRoot->FirstChildElement("Bricks")->GetText()));

	return level;
}

std::vector<std::string> LevelGenerator::parseLevelLayout(std::string unparsedLayout) {
	std::vector<std::string> layout;
	boost::split(layout, unparsedLayout, boost::is_any_of("\n"));
	return layout;
}