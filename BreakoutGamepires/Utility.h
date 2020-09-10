#pragma once

#include <cmath>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>



bool isSpaceNewlineOrTab(char c);

std::string removeSpaces(std::string str);

bool fileExists(std::string filename);

template<typename T>
sf::Vector2<T> clamp(sf::Vector2<T> vectorToClamp, sf::Vector2<T> clampValues) {
	T x = std::max(-clampValues.x, std::min(clampValues.x, vectorToClamp.x));
	T y = std::max(-clampValues.y, std::min(clampValues.y, vectorToClamp.y));
	return sf::Vector2<T>(x, y);
}

template<typename T>
T dotProduct(sf::Vector2<T> vectorA, sf::Vector2<T> vectorB) {
	return sqrt((vectorA.x * vectorB.x) + (vectorA.y * vectorB.y));
}

template<typename T>
T magnitude(sf::Vector2<T> vector) {
	return dotProduct(vector, vector);
}

template<typename T>
sf::Vector2<T> normalize(sf::Vector2<T> vector) {
	T mag = magnitude(vector);
	return sf::Vector2<T>(vector.x / mag, vector.y / mag);
}