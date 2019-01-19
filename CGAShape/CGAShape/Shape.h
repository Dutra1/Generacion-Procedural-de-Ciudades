#pragma once
#ifndef SHAPE
#define SHAPE
#include "glm/glm.hpp"
#include "Box.h"
#include "Floorplan.h"
#include "Ratio.h"
#include "BaseDistribution.h"

class Shape : public Box{
protected:
	std::string symbol;

	//What an ugly way to do this
	std::vector<glm::vec2> floor;
public:
	Shape(const std::string & symbol = "");

	void setSymbol(const std::string & symbol);
	std::string getSymbol() const;

	void applyFloorplan(const Floorplan & floorplan);
	void copyFloor(const Shape & shapeWithFloor);
	bool hasFloorplan() const;
	std::size_t getFloorPointCount() const;
	glm::vec2 getFloorPoint(std::size_t index) const;

	std::vector<Shape> split(Axis axis, const std::vector<Ratio> & ratios);
	std::vector<Shape> repeat(Axis axis, float size, const std::string & symbol);
	std::vector<Shape> repeat(Axis axis, float size, const std::string & symbol, const BaseDistribution & distribution, std::default_random_engine & generator);
	std::vector<Shape> component(Component component, std::string symbol);
};

#endif
