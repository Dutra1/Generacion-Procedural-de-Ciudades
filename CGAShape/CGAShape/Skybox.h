#pragma once
#ifndef SKYBOX
#define SKYBOX
#include "GeometryDrawable.h"
#include "MaterialHolder.h"

#define SKYBOX_DISTANCE 5000

class Skybox {
private:
	GeometryDrawable skyboxGD;
public:
	Skybox(GLuint program);
	void draw(MaterialHolder & materialHolder, GLuint uniColor) const;
	void quit();
};

#endif

