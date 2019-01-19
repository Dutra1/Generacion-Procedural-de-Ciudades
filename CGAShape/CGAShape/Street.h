#pragma once
#ifndef STREET
#define STREET

class Street {
private:
	std::vector<glm::vec3> points;
	double width;
public:
	Street(double width);
	Street(const std::vector<glm::vec3> & points, double width);

	void addPoint(const glm::vec3 & point);

	std::size_t getPointCount() const;
	glm::vec3 getPoint(std::size_t index) const;
	double getWidth() const;
};

#endif