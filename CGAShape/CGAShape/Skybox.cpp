#include "stdafx.h"
#include "Skybox.h"

Skybox::Skybox(GLuint program) {
	//LEFT
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 0 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 0 / 4.0, 2 / 3.0 }, -1);

	//FRONT
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 2 / 3.0 }, -1);

	//RIGHT
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 3 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 3 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 2 / 3.0 }, -1);

	//BACK
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 3 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 4 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 4 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 3 / 4.0, 2 / 3.0 }, -1);

	//UP
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 2 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 3 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE, -SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 3 / 3.0 }, -1);

	//DOWN
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 0 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE, -SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 0 / 3.0 }, -1);
	skyboxGD.insertPoint({  SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 2 / 4.0, 1 / 3.0 }, -1);
	skyboxGD.insertPoint({ -SKYBOX_DISTANCE,  SKYBOX_DISTANCE,  SKYBOX_DISTANCE }, { 0, -1, 0 }, { 1 / 4.0, 1 / 3.0 }, -1);

	//Elements
	for (std::size_t i = 0; i < 6; i++) {
		skyboxGD.addElements({ 0, 1, 2, 0, 2, 3 });
	}
	
	skyboxGD.createBuffers(program);
}

void Skybox::draw(MaterialHolder & materialHolder, GLuint uniColor) const {
	materialHolder.bindMaterial("Skybox", uniColor);
	skyboxGD.draw();
}

void Skybox::quit() {
	skyboxGD.quit();
}