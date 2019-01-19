#include "stdafx.h"
#include "Street.h"

Street::Street(double width) : width(width) {};

Street::Street(const std::vector<glm::vec3> & points, double width) : points(points), width(width) {};

void Street::addPoint(const glm::vec3 & point) {
	points.push_back(point);
}

std::size_t Street::getPointCount() const {
	return points.size();
}

glm::vec3 Street::getPoint(std::size_t index) const {
	if (index < points.size()) {
		return points.at(index);
	} else {
		std::cerr << "Accessed wrong street point" << std::endl;
		return glm::vec3();
	}
}

double Street::getWidth() const {
	return width;
}