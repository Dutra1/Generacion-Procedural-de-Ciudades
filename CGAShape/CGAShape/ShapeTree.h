#pragma once
#ifndef SHAPE_TREE
#define SHAPE_TREE
#include "Shape.h"

class ShapeTree {
private:
	Shape shape;

	ShapeTree * parent;
	std::vector<ShapeTree> children;

	std::size_t siblingIndex;
	std::size_t depth;
public:
	//Tree functions
	ShapeTree(const Shape & shape = Shape(), ShapeTree * parent = nullptr, std::size_t siblingIndex = 0, std::size_t depth = 0);

	void addChild(const Shape & child);
	void addChildren(const std::vector<Shape> & children);

	bool isRoot() const;
	bool isLeaf() const;

	const Shape & getShape() const;
	Shape & getShape();

	const ShapeTree & getParent() const;
	ShapeTree & getParent();

	const ShapeTree & getChild(std::size_t index) const;
	ShapeTree & getChild(std::size_t index);
	const ShapeTree & operator[](std::size_t index) const;
	ShapeTree & operator[](std::size_t index);

	std::size_t getChildrenCount() const;
	std::size_t getSiblingIndex() const;

};

#endif

