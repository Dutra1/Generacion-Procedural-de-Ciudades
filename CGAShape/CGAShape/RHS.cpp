#include "stdafx.h"
#include "RHS.h"

RHS::RHS() : rule([](ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables, std::stack<Shape> & stack) {
	Shape shape = Shape();
	shape.resize(tree.getShape().getSize());
	return shape;
}) {};

void RHS::applyRule(ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables) const {
	rule(tree, generator, variables, std::stack<Shape>());
}

/***** Box Rules *****/

#define AddToRule(addition) \
	auto oldRule = rule;\
	rule = [=](ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables, std::stack<Shape> & stack) {\
		Shape shape = oldRule(tree, generator, variables, stack);\
		addition\
		return shape;\
	}

void RHS::translate(const Argument & x, const Argument & y, const Argument & z) {
	AddToRule(
		shape.translate(x.getValue(shape, generator, variables), y.getValue(shape, generator, variables), z.getValue(shape, generator, variables));
	);
}

void RHS::translate(Axis axis, const Argument & value) {
	AddToRule(
		shape.translate(axis, value.getValue(shape, generator, variables));
	);
}

void RHS::relativeTranslate(const Argument & x, const Argument & y, const Argument & z) {
	AddToRule(
		shape.relativeTranslate(x.getValue(shape, generator, variables), y.getValue(shape, generator, variables), z.getValue(shape, generator, variables));
	);
}

void RHS::relativeTranslate(Axis axis, const Argument & value) {
	AddToRule(
		shape.relativeTranslate(axis, value.getValue(shape, generator, variables));
	);
}

void RHS::rotate(Axis axis, const Argument & angle, bool center) {
	AddToRule(
		shape.rotate(axis, angle.getValue(shape, generator, variables), center);
	);
}

void RHS::resize(const Argument & x, const Argument & y, const Argument & z, bool center) {
	AddToRule(
		shape.resize(x.getValue(shape, generator, variables), y.getValue(shape, generator, variables), z.getValue(shape, generator, variables), center);
	);
}

void RHS::resize(Axis axis, const Argument & value, bool center) {
	AddToRule(
		shape.resize(axis, value.getValue(shape, generator, variables), center);
	);
}

void RHS::scale(const Argument & x, const Argument & y, const Argument & z, bool center) {
	AddToRule(
		shape.scale(x.getValue(shape, generator, variables), y.getValue(shape, generator, variables), z.getValue(shape, generator, variables), center);
	);
}

void RHS::scale(Axis axis, const Argument & value, bool center) {
	AddToRule(
		shape.scale(axis, value.getValue(shape, generator, variables), center);
	);
}

void RHS::scale(const Argument & value, bool center) {
	AddToRule(
		shape.scale(value.getValue(shape, generator, variables), center);
	);
}

void RHS::mirror(Axis axis) {
	AddToRule(
		shape.mirror(axis);
	);
}

void RHS::flatten(Axis axis) {
	AddToRule(
		shape.flatten(axis);
	);
}

void RHS::view(Component component) {
	AddToRule(
		shape.view(component);
	);
}

void RHS::view(Axis face, bool faceDirection, Axis up, bool upDirection) {
	AddToRule(
		shape.view(face, faceDirection, up, upDirection);
	);
}

/***** Tree Rules *****/

void RHS::instantiate(const std::string & symbol) {
	AddToRule(
		shape.setSymbol(symbol);
		tree.addChild(shape);
	);
}

void RHS::split(Axis axis, const std::vector<ArgumentRatio> & ratios) {
	AddToRule(
		std::vector<Ratio> simpleRatios;
		for (std::size_t i = 0; i < ratios.size(); ++i) {
			simpleRatios.push_back(ratios[i].toRatio(shape, generator, variables));
		}
		tree.addChildren(shape.split(axis, simpleRatios));
	);
}

void RHS::repeat(Axis axis, const Argument & size, const std::string & symbol) {
	AddToRule(
		tree.addChildren(shape.repeat(axis, size.getValue(shape, generator, variables), symbol));
	);
}

void RHS::repeat(Axis axis, const Argument & size, const std::string & symbol, const BaseDistribution & distribution) {
	AddToRule(
		tree.addChildren(shape.repeat(axis, size.getValue(shape, generator, variables), symbol, distribution, generator));
	);
}

void RHS::component(Component component, const std::string & symbol) {
	AddToRule(
		tree.addChildren(shape.component(component, symbol));
	);
}

void RHS::inheritFloorPlan() {
	AddToRule(
		shape.copyFloor(tree.getShape());
	);
}

void RHS::clearAndSetVariable(const std::string & name, const Argument & value) {
	AddToRule(
		variables[name] = value.getValue(tree.getShape(), generator, variables);
	);
}

void RHS::setVariable(const std::string & name, const Argument & value) {
	AddToRule(
		if (variables.count(name) == 0) {
			variables[name] = value.getValue(tree.getShape(), generator, variables);
		}
	);
}

void RHS::clearVariable(const std::string & name) {
	AddToRule(
		variables.erase(name);
	);
}

void RHS::push() {
	AddToRule(
		stack.push(shape);
	);
}

void RHS::pop() {
	AddToRule(
		if (stack.size() > 0) {
			shape = stack.top();
			stack.pop();
		}
	);
}
