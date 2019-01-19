#include "stdafx.h"
#include "TreeNode.h"

ShapeTree::ShapeTree(const Shape & shape) : shape(shape) {}

ShapeTree ShapeTree::setParent(const std::shared_ptr<const ShapeTree> parent) {
	this->parent = parent;
	return *this;
}

ShapeTree ShapeTree::addChild(const std::shared_ptr<ShapeTree> child) {
	children.push_back(child);
	child->setParent(this);
	return *this;
}