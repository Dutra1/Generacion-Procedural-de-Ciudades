#pragma once
#ifndef USINGS
#define USINGS
#include "stdafx.h"
#include "Shape.h"

#define ShapeParameters const Shape & shape, std::default_random_engine & generator, const std::map<std::string, double> & variables
#define ShapePassParameters shape, generator, variables
#define LambdaShape(returnValue) [=](ShapeParameters) { return (returnValue); }
using ShapeArgument  = std::function<double(ShapeParameters)>;
using ShapeCondition = std::function< bool (ShapeParameters)>;

#endif