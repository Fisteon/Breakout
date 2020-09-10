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
	QuadTree(float _left, float _top, float _width, float _height, int _level, QuadTree* _parent = nullptr);
	bool contains(Brick* b);
	bool contains(Ball b);
	bool contains(float o_left, float o_top, float o_width, float o_height); // Object_left,top,width,height
	void deleteBrick(Brick* brickToDelete);

	std::unordered_set<Brick*> merge();
	std::unordered_set<Brick*> collisionBricks(Ball ball);

	std::unordered_set<Brick*> bricks;
	
	QuadTree*	parent;
	QuadTree*	NW;
	QuadTree*	NE;
	QuadTree*	SW;
	QuadTree*	SE;

	float	left;
	float	top;
	float	width;
	float	height;

	int		level;
	int		totalElementsContained;

	static int const maxLevel = 5;
	static int const maxCapacity = 7;

	void addBrick(Brick* b);
	sf::RectangleShape bounds;
};