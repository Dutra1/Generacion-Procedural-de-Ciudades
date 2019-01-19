#pragma once
#ifndef MAIN_HOLDER
#define MAIN_HOLDER
#include "ProgramHolder.h"
#include "ShaderHolder.h"
#include "MaterialHolder.h"
#include "ShapeInfoHolder.h"

class MainHolder {
private:
	ProgramHolder programHolder;
	ShaderHolder shaderHolder;
	MaterialHolder materialHolder;
	ShapeInfoHolder shapeInfoHolder;
public:
	void initialize();
	ProgramHolder & getProgramHolder();
	ShaderHolder & getShaderHolder();
	MaterialHolder & getMaterialHolder();
	ShapeInfoHolder & getShapeInfoHolder();
	void quit();
};

#endif

