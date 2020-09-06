#include "Utility.h"

bool isSpaceNewlineOrTab(char c) {
	return c == '\n' || c == ' ' || '\t';
}

std::string removeSpaces(std::string str) {
	boost::trim_if(str, boost::is_any_of("\n,\t, "));
	boost::replace_all(str, " ", "");
	return str;
}

bool fileExists(std::string filename) {
	return boost::filesystem::exists(filename);
}