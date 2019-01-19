#include "stdafx.h"
#include "Argument.h"

Argument::Argument(double value) : argument(LambdaShape(value)) {}

Argument::Argument(const char* variable) : argument(LambdaShape(variables.count(variable) > 0 ? variables.at(variable) : 0.0)) {}

Argument::Argument(const std::string & variable) : argument(LambdaShape(variables.at(variable))) {}

Argument::Argument(Axis axis) : argument(LambdaShape(shape.getSize(axis))) {}

Argument::Argument(const BaseDistribution & distribution) : argument(LambdaShape(distribution.use(generator))) {}

Argument::Argument(const ShapeArgument & operation) : argument(operation) {}

double Argument::getValue(ShapeParameters) const {
	return argument(ShapePassParameters);
}