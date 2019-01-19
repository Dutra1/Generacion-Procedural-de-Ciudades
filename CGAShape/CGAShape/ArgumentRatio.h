#pragma once
#ifndef ARGUMENT_RATIO
#define ARGUMENT_RATIO
#include "stdafx.h"
#include "Ratio.h"
#include "Argument.h"
#include "Shape.h"

struct ArgumentRatio {
public:
	Argument argument;
	bool isAbsolute;
	std::string symbol;

	Ratio toRatio(ShapeParameters) const {
		return{ argument.getValue(ShapePassParameters), isAbsolute, symbol };
	}
};

#endif