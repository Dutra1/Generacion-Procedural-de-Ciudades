#include "stdafx.h"
#include "MainHolder.h"
#include "ShaderRepository.h"

void MainHolder::initialize() {
	GLuint vertexShader = shaderHolder.compileVertexShader("singleColorVertex3D", ShaderRepository::singleColorVertex3D);
	GLuint fragmentShader = shaderHolder.compileFragmentShader("singleColorFragment3D", ShaderRepository::singleColorFragment3D);
	programHolder.createProgram("singleColor3D", vertexShader, fragmentShader);
}

ProgramHolder & MainHolder::getProgramHolder() {
	return programHolder;
}

ShaderHolder & MainHolder::getShaderHolder() {
	return shaderHolder;
}

MaterialHolder & MainHolder::getMaterialHolder() {
	return materialHolder;
}

ShapeInfoHolder & MainHolder::getShapeInfoHolder() {
	return shapeInfoHolder;
}

void MainHolder::quit() {
	programHolder.quit();
	shaderHolder.quit();
	materialHolder.quit();
}