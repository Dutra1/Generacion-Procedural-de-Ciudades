#pragma once
#ifndef RIGHT_HAND_SIDE
#define RIGHT_HAND_SIDE
#include "ShapeTree.h"
#include "Component.h"
#include "BaseDistribution.h"
#include "Argument.h"
#include "ArgumentRatio.h"

class RHS {
private:
	std::function<Shape(ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables, std::stack<Shape> & stack)> rule;
public:
	RHS();

	void applyRule(ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables) const;

	//Box rules
	void translate(const Argument & x, const Argument & y, const Argument & z);
	void translate(Axis axis, const Argument & value);
	void relativeTranslate(const Argument & x, const Argument & y, const Argument & z);
	void relativeTranslate(Axis axis, const Argument & value);

	void rotate(Axis axis, const Argument & angle, bool center = true);

	void resize(const Argument & x, const Argument & y, const Argument & z, bool center = false);
	void resize(Axis axis, const Argument & value, bool center = false);
	void scale(const Argument & x, const Argument & y, const Argument & z, bool center = false);
	void scale(Axis axis, const Argument & value, bool center = false);
	void scale(const Argument & value, bool center = false);

	void mirror(Axis axis);
	void flatten(Axis axis);

	void view(Component component);
	void view(Axis face, bool faceDirection, Axis up, bool upDirection);

	//Tree rules
	void instantiate(const std::string & symbol);
	void split(Axis axis, const std::vector<ArgumentRatio> & ratios);
	void repeat(Axis axis, const Argument & size, const std::string & symbol);
	void repeat(Axis axis, const Argument & size, const std::string & symbol, const BaseDistribution & distribution);
	void component(Component component, const std::string & symbol);
	void inheritFloorPlan();

	//Variable Rules
	void clearAndSetVariable(const std::string & name, const Argument & value);
	void setVariable(const std::string & name, const Argument & value);
	void clearVariable(const std::string & name);

	//Stack rules
	void push();
	void pop();
};

#endif
