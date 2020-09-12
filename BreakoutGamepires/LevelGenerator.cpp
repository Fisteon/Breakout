#include "LevelGenerator.h"
#include "Utility.h"

LevelGenerator::LevelGenerator() {

}

Level LevelGenerator::parseLevelInfo(int levelNumber) {
	Level level;
	XMLDocument levelInfo;

	if (levelInfo.LoadFile(("Resources/Levels/LevelData_" + std::to_string(levelNumber) + ".xml").c_str()) != XML_SUCCESS) {
		std::cout << "Error loading level data file.";
		return level;
	}
	XMLNode * docRoot = levelInfo.FirstChild();

	const char* temp;

	docRoot->ToElement()->QueryIntAttribute("RowCount",				&level.rowCount);
	docRoot->ToElement()->QueryIntAttribute("ColumnCount",			&level.columnCount);
	docRoot->ToElement()->QueryIntAttribute("RowSpacing",			&level.rowSpacing);
	docRoot->ToElement()->QueryIntAttribute("ColumnSpacing",		&level.columnSpacing);
	docRoot->ToElement()->QueryStringAttribute("BackgroundTexture", &temp); level.backgroundTexture = temp;
	docRoot->ToElement()->QueryStringAttribute("SpriteTexture",		&temp);	level.spriteTexture = temp;

	XMLElement* sounds = docRoot->FirstChildElement("SoundPaths");

	sounds->QueryStringAttribute("Hit",			&temp); level.soundPaths["Hit"] = temp;
	sounds->QueryStringAttribute("Break",		&temp); level.soundPaths["Break"] = temp;
	sounds->QueryStringAttribute("InfiniteHit", &temp); level.soundPaths["InfiniteHit"] = temp;
	sounds->QueryStringAttribute("BallBounce",	&temp); level.soundPaths["BallBounce"] = temp;
	sounds->QueryStringAttribute("BallMiss",	&temp); level.soundPaths["BallMiss"] = temp;
	sounds->QueryStringAttribute("Powerup",		&temp); level.soundPaths["Powerup"] = temp;
	sounds->QueryStringAttribute("Levelup",		&temp); level.soundPaths["Levelup"] = temp;

	XMLElement* bricktypes = docRoot->FirstChildElement("BrickTypes");

	for (XMLElement* b = bricktypes->FirstChildElement(); b != NULL; b = b->NextSiblingElement()) {
		Brick brick;
		const char* brickID;
		b->QueryStringAttribute("Id",			&brickID);

		b->QueryIntAttribute("TextureX",		&brick.textureX);
		b->QueryIntAttribute("TextureY",		&brick.textureY);
		b->QueryIntAttribute("TexturesLeft",	&brick.texturesLeft);
		b->QueryIntAttribute("HitPoints",		&brick.hp);
		b->QueryIntAttribute("BreakScore",		&brick.breakScore);

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