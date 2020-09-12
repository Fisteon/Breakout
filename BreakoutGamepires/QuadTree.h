#pragma once

#include <unordered_set>
#include <vector>

#include <SFML\Graphics.hpp>
#include "Brick.h"
#include "Ball.h"

class QuadTree {
public:
	QuadTree();
	~QuadTree();
	QuadTree(
		float _left, 
		float _top, 
		float _width, 
		float _height, 
		int _level, 
		QuadTree* _parent = nullptr
	);

	void addBrick(Brick* b);
	void deleteBrick(Brick* brickToDelete);
	std::unordered_set<Brick*> collisionBricks(Ball ball);

	bool isSplit() { return this->NW != nullptr; };

	QuadTree*	parent;
	QuadTree*	NW;
	QuadTree*	NE;
	QuadTree*	SW;
	QuadTree*	SE;

	sf::RectangleShape bounds;
	
private:
	void split();
	std::unordered_set<Brick*> merge();
	bool contains(sf::Shape* shape);
	bool contains(float o_left, float o_top, float o_width, float o_height); // Object_left,top,width,height

	std::unordered_set<Brick*> bricks;

	float	left;
	float	top;
	float	width;
	float	height;

	int		level;
	int		totalElementsContained;

	static int const maxLevel = 5;
	static int const maxCapacity = 8;
};