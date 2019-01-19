#include "stdafx.h"
#include "Floor.h"

Floor::Floor(GLuint program) {
	//FLOOR
	floorGD.insertPoint({ -FLOOR_DISTANCE,  0, -FLOOR_DISTANCE }, { 0, 1, 0 }, { 0, 0 }, -10);
	floorGD.insertPoint({  FLOOR_DISTANCE,  0, -FLOOR_DISTANCE }, { 0, 1, 0 }, { 0, FLOOR_DIVIDER }, -10);
	floorGD.insertPoint({  FLOOR_DISTANCE,  0,  FLOOR_DISTANCE }, { 0, 1, 0 }, { FLOOR_DIVIDER, FLOOR_DIVIDER }, -10);
	floorGD.insertPoint({ -FLOOR_DISTANCE,  0,  FLOOR_DISTANCE }, { 0, 1, 0 }, { FLOOR_DIVIDER, 0 }, -10);

	//Elements
	floorGD.addElements({ 0, 1, 2, 0, 2, 3 });

	floorGD.createBuffers(program);
}

void Floor::draw(MaterialHolder & materialHolder, GLuint uniColor) const {
	materialHolder.bindMaterial("Floor", uniColor);
	floorGD.draw();
}

void Floor::quit() {
	floorGD.quit();
}

