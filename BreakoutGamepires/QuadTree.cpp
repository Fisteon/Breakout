#include "QuadTree.h"

QuadTree::QuadTree() {

}

QuadTree::~QuadTree() {

}

QuadTree::QuadTree(float _left, float _top, float _width, float _height, int _level, QuadTree* _parent) :
	left(_left), top(_top), width(_width), height(_height), level(_level), parent(_parent) {
	if (level == 0) {
		NW = new QuadTree(left, top, width / 2.0f, height / 2.0f, level + 1, this);
		NE = new QuadTree(left + width / 2.0f, top, width / 2.0f, height / 2.0f, level + 1, this);
		SW = new QuadTree(left, top + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, this);
		SE = new QuadTree(left + width / 2.0f, top + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, this);
	}
	totalElementsContained = 0;
	bounds.setPosition(left, top);
	bounds.setSize(sf::Vector2f(width, height));
	bounds.setFillColor(sf::Color(sf::Color::Transparent));
	bounds.setOutlineThickness(-1.f);
	bounds.setOutlineColor(sf::Color(sf::Color::Red));
}

void QuadTree::addBrick(Brick* newBrick) {
	if (!this->contains(newBrick)) return;

	totalElementsContained++;
	if (NW != nullptr) {
		NW->addBrick(newBrick);
		NE->addBrick(newBrick);
		SW->addBrick(newBrick);
		SE->addBrick(newBrick);

		return;
	}

	if (bricks.size() >= maxCapacity && level <= maxLevel) {
		NW = new QuadTree(left, top, width / 2.0f, height / 2.0f, level + 1, this);
		NE = new QuadTree(left + width / 2.0f, top, width / 2.0f, height / 2.0f, level + 1, this);
		SW = new QuadTree(left, top + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, this);
		SE = new QuadTree(left + width / 2.0f, top + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, this);

		bricks.insert(newBrick);
		for (Brick* b : bricks) {
			NW->addBrick(b);
			NE->addBrick(b);
			SW->addBrick(b);
			SE->addBrick(b);
		}
		bricks.clear();
	}
	else {
		bricks.insert(newBrick);
	}
}

void QuadTree::deleteBrick(Brick* brickToDelete) {
	if (!this->contains(brickToDelete)) return;

	totalElementsContained--;
	if (NW != nullptr && totalElementsContained <= maxCapacity && this->level != 0){
		bricks = merge();
		bricks.erase(brickToDelete);
		return;
	}

	if (NW != nullptr) {
		NW->deleteBrick(brickToDelete);
		NE->deleteBrick(brickToDelete);
		SW->deleteBrick(brickToDelete);
		SE->deleteBrick(brickToDelete);

		return;
	}

	bricks.erase(brickToDelete);
}

// Recursively searches all nodes for the one containing the ball and
// returns bricks in that node to check for collision
std::unordered_set<Brick*> QuadTree::collisionBricks(Ball ball) {
	std::unordered_set<Brick *> bricksNearBall;
	if (!this->contains(ball)) return bricksNearBall;

	if (NW != nullptr) {
		std::unordered_set<Brick *> temp;
		temp = NW->collisionBricks(ball);
		bricksNearBall.insert(temp.begin(), temp.end());
		temp = NE->collisionBricks(ball);
		bricksNearBall.insert(temp.begin(), temp.end());
		temp = SW->collisionBricks(ball);
		bricksNearBall.insert(temp.begin(), temp.end());
		temp = SE->collisionBricks(ball);
		bricksNearBall.insert(temp.begin(), temp.end());

		return bricksNearBall;
	}

	return bricks;
}

// Removes child nodes from a parent if the combined amount of bricks
// in all children is less than max brick limit
std::unordered_set<Brick*> QuadTree::merge() {
	if (this->NW != nullptr) {
		std::unordered_set<Brick*> childBricks;
		std::unordered_set<Brick*> temp;
		temp = this->NW->merge();
		childBricks.insert(temp.begin(), temp.end());
		temp = this->NE->merge();
		childBricks.insert(temp.begin(), temp.end());
		temp = this->SW->merge();
		childBricks.insert(temp.begin(), temp.end());
		temp = this->SE->merge();
		childBricks.insert(temp.begin(), temp.end());
		delete(this->NW); this->NW = nullptr;
		delete(this->NE); this->NE = nullptr;
		delete(this->SW); this->SW = nullptr;
		delete(this->SE); this->SE = nullptr;
		return childBricks;
	}
	return this->bricks;
}

bool QuadTree::contains(Brick *b) {
	return contains(
		b->getGlobalBounds().left,
		b->getGlobalBounds().top,
		b->getGlobalBounds().width,
		b->getGlobalBounds().height
	);
}

bool QuadTree::contains(Ball b) {
	return contains(
		b.getGlobalBounds().left,
		b.getGlobalBounds().top,
		b.getGlobalBounds().width,
		b.getGlobalBounds().height
	);
}

bool QuadTree::contains(float o_left, float o_top, float o_width, float o_height) {
	return (
		// (A + C) * (B + D)
		((o_left > left && o_left < left + width) ||
		(o_left + o_width > left && o_left + o_width < left + width)) &&
		((o_top > top && o_top < top + height) ||
		(o_top + o_height > top && o_top + o_height < top + height))
		);
		/*((o_left > left && o_left < left + width) &&						// A
		(o_top > top && o_top < top + height)) ||							// B
		((o_left + o_width > left && o_left + o_width < left + width) &&	// C
		(o_top > top && o_top < top + height)) ||							// B
		((o_left > left && o_left < left + width) &&						// A
		(o_top + o_height > top && o_top + o_height < top + height)) ||		// D
		((o_left + o_width > left && o_left + o_width < left + width) &&	// C
		(o_top + o_height > top && o_top + o_height < top + height))		// D		*/
}

