#pragma once
#ifndef FLOOR
#define FLOOR
#include "GeometryDrawable.h"
#include "MaterialHolder.h"

#define FLOOR_DISTANCE	6000
#define FLOOR_SECTION	10
#define FLOOR_DIVIDER	(FLOOR_DISTANCE / FLOOR_SECTION)

class Floor {
private:
	GeometryDrawable floorGD;
public:
	Floor(GLuint program);
	void draw(MaterialHolder & materialHolder, GLuint uniColor) const;
	void quit();
};

#endif

