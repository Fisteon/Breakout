#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
//#include <string>


bool isSpaceNewlineOrTab(char c);

std::string removeSpaces(std::string str);

bool fileExists(std::string filename);