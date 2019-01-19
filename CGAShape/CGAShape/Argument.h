#pragma once
#ifndef ARGUMENT
#define ARGUMENT
#include "Usings.h"
#include "Shape.h"
#include "BaseDistribution.h"

class Argument {
private:
	ShapeArgument argument;
public:
	Argument(double value);
	Argument(const char* variable);
	Argument(const std::string & variable);
	Argument(Axis axis);
	Argument(const BaseDistribution & distribution);
	Argument(const ShapeArgument & operation);

	double getValue(ShapeParameters) const;
};

#endif