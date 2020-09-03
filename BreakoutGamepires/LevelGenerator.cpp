#include <iostream>

#include "Utility.h"
#include "LevelGenerator.h"
#include "Level.h"

LevelGenerator::LevelGenerator(int levelNumber) {
	Level level;
	level.rowCount;
	std::map<const char*, Brick> brickTypes;
	
	XMLDocument levelInfo;	
	
	std::string filename = "LevelData_" + std::to_string(levelNumber) + ".xml";

	levelInfo.LoadFile(filename.c_str());
	XMLNode * docRoot = levelInfo.FirstChild();

	const char* texture;

	docRoot->ToElement()->QueryIntAttribute("RowCount", &level.rowCount);
	docRoot->ToElement()->QueryIntAttribute("ColumnCount", &level.columnCount);
	docRoot->ToElement()->QueryIntAttribute("RowSpacing", &level.rowSpacing);
	docRoot->ToElement()->QueryIntAttribute("ColumnSpacing", &level.columnSpacing);
	docRoot->ToElement()->QueryStringAttribute("BackgroundTexture", &texture);
	level.backgroundTexture = texture;

	XMLElement* bricktypes = docRoot->FirstChildElement("BrickTypes");

	for (XMLElement* b = bricktypes->FirstChildElement(); b != NULL; b = b->NextSiblingElement()) {
		Brick brick;
		const char* brickID;
		b->QueryStringAttribute	("Id",			&brickID);
		b->QueryStringAttribute	("Texture",		&brick.texture);
		b->QueryIntAttribute	("HitPoints",	&brick.hp);
		b->QueryStringAttribute	("HitSound",	&brick.hitSound);
		b->QueryStringAttribute	("BreakSound",	&brick.breakSound);
		b->QueryIntAttribute	("BreakScore",	&brick.breakScore);

		brickTypes["S"] = brick;
	}
	
	level.brickTypes = brickTypes;

	const char* brickLayout = docRoot->FirstChildElement("Bricks")->GetText();
	std::string x = brickLayout;
	boost::trim_right(x);
	//x = removeSpaces(x);

	return;
}