#include <iostream>

#include "Utility.h"
#include "LevelGenerator.h"

LevelGenerator::LevelGenerator() {

}

Level LevelGenerator::parseLevelInfo(int levelNumber) {
	Level level;
	XMLDocument levelInfo;

	if (levelInfo.LoadFile(("LevelData_" + std::to_string(levelNumber) + ".xml").c_str()) != XML_SUCCESS) {
		std::cout << "Error loading level data file.";
		return level;
	}
	XMLNode * docRoot = levelInfo.FirstChild();

	const char* temp;

	docRoot->ToElement()->QueryIntAttribute("RowCount", &level.rowCount);
	docRoot->ToElement()->QueryIntAttribute("ColumnCount", &level.columnCount);
	docRoot->ToElement()->QueryIntAttribute("RowSpacing", &level.rowSpacing);
	docRoot->ToElement()->QueryIntAttribute("ColumnSpacing", &level.columnSpacing);
	docRoot->ToElement()->QueryStringAttribute("BackgroundTexture", &temp);
	level.backgroundTexture = temp;

	XMLElement* bricktypes = docRoot->FirstChildElement("BrickTypes");

	for (XMLElement* b = bricktypes->FirstChildElement(); b != NULL; b = b->NextSiblingElement()) {
		Brick brick;
		const char* brickID;
		b->QueryStringAttribute("Id", &brickID);

		brick.texture = getXMLStringAttribute("Texture", b);
		brick.hitSound = getXMLStringAttribute("HitSound", b);
		brick.breakSound = getXMLStringAttribute("BreakSound", b);

		b->QueryIntAttribute("HitPoints", &brick.hp);
		b->QueryIntAttribute("BreakScore", &brick.breakScore);

		level.brickTypes[brickID[0]] = brick;
	}

	level.levelLayout = parseLevelLayout(removeSpaces(docRoot->FirstChildElement("Bricks")->GetText()));

	return level;
}

std::vector<std::string> LevelGenerator::parseLevelLayout(std::string unparsedLayout) {
	std::vector<std::string> layout;
	boost::split(layout, unparsedLayout, boost::is_any_of("\n"));
	return layout;
}

std::string LevelGenerator::getXMLStringAttribute(std::string attribute, XMLElement* element) {
	const char* temp;
	if (element->QueryStringAttribute(attribute.c_str(), &temp) != XML_NO_ATTRIBUTE) {
		return temp;
	}
	return "";
}