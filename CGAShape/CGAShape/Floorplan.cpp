#include "stdafx.h"
#include "Floorplan.h"

Floorplan::Floorplan(const std::vector<glm::vec2> & coordinates) {
	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;

	for (std::size_t i = 0; i < coordinates.size(); ++i) {
		float x = coordinates[i].x;
		float y = coordinates[i].y;
		
		minX = std::min(minX, x);
		maxX = std::max(maxX, x);
		minY = std::min(minY, y);
		maxY = std::max(maxY, y);
	}

	float scaleX = maxX - minX;
	float scaleY = maxY - minY;

	position = glm::vec2(minX, minY);
	size = glm::vec2(scaleX, scaleY);

	//Transform and save the points
	glm::mat3 pointTransform = glm::mat3();
	pointTransform = glm::scale(pointTransform, glm::vec2(scaleX != 0 ? 1.0 / scaleX : 0.0f, scaleY != 0 ? 1.0 / scaleY : 0.0f));
	pointTransform = glm::translate(pointTransform, glm::vec2(-minX, -minY));
	for (std::size_t i = 0; i < coordinates.size(); ++i) {
		floor.push_back(glm::vec2(pointTransform * glm::vec3(coordinates[i], 1.0f)));
	}
}

glm::vec2 Floorplan::getPosition() const {
	return position;
}

glm::vec2 Floorplan::getSize() const {
	return size;
}

std::vector<glm::vec2> Floorplan::getFloor() const {
	return floor;
}