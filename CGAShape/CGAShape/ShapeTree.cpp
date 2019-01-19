#include "stdafx.h"
#include "ShapeTree.h"

ShapeTree::ShapeTree(const Shape & shape, ShapeTree * parent, std::size_t siblingIndex, std::size_t depth)
	: shape(shape), parent(parent), siblingIndex(siblingIndex), depth(depth) {};

void ShapeTree::addChild(const Shape & child) {
	ShapeTree childNode = ShapeTree(child, this, children.size(), depth + 1);
	children.push_back(childNode);
}

void ShapeTree::addChildren(const std::vector<Shape> & children) {
	for (std::size_t i = 0; i < children.size(); i++) {
		addChild(children[i]);
	}
}

bool ShapeTree::isRoot() const {
	return parent == nullptr;
}

bool ShapeTree::isLeaf() const {
	return children.size() == 0;
}

const Shape & ShapeTree::getShape() const {
	return shape;
}

Shape & ShapeTree::getShape() {
	return shape;
}

const ShapeTree & ShapeTree::getParent() const {
	return *parent;
}

ShapeTree & ShapeTree::getParent() {
	return *parent;
}

const ShapeTree & ShapeTree::getChild(std::size_t index) const {
	return children.at(index);
}

ShapeTree & ShapeTree::getChild(std::size_t index) {
	return children.at(index);
}

const ShapeTree & ShapeTree::operator[](std::size_t index) const {
	return getChild(index);
}

ShapeTree & ShapeTree::operator[](std::size_t index) {
	return getChild(index);
}

std::size_t ShapeTree::getChildrenCount() const {
	return children.size();
}

std::size_t ShapeTree::getSiblingIndex() const {
	return siblingIndex;
}