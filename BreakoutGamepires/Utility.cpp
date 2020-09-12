#include "Utility.h"

inline bool isSpaceNewlineOrTab(char c) {
	return c == '\n' || c == ' ' || '\t';
}

bool fileExists(std::string filename) {
	return boost::filesystem::exists(filename);
}

std::string removeSpaces(std::string str) {
	boost::trim_if(str, boost::is_any_of("\n,\t, "));
	boost::replace_all(str, " ", "");
	return str;
}

int getVectorDirection(sf::Vector2f vector) {
	// Direction order -> LEFT RIGHT UP DOWN
	sf::Vector2f directions[] = {
		sf::Vector2f(-1.f, 0.f),
		sf::Vector2f(1.f,  0.f),
		sf::Vector2f(0.f,  1.f),
		sf::Vector2f(0.f, -1.f)
	};

	float max = 0.0f;
	int bestMatch = -1;
	for (int i = 0; i < 4; i++) {
		float dotProd = dotProduct(vector, directions[i]);
		if (dotProd > max) {
			max = dotProd;
			bestMatch = i;
		}
	}
	return bestMatch;
}