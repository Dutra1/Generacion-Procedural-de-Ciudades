#include "stdafx.h"
#include "Utilities.h"

double Utilities::dotProduct(const glm::vec2 & p1, const glm::vec2 & p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

double Utilities::crossProduct(const glm::vec2 & p1, const glm::vec2 & p2) {
	return p1.x * p2.y - p1.y * p2.x;
}

float Utilities::hypotenuse(float a, float b) {
	return sqrt(pow(a, 2) + pow(b, 2));
}

double Utilities::distance(const glm::vec2 & a, const glm::vec2 & b) {
	return hypotenuse(b.y - a.y, b.x - a.x);
}

bool Utilities::doSegmentsIntersect(const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & d) {
	glm::vec2 r = b - a;
	glm::vec2 s = d - c;

	double numerator = crossProduct(c - a, r);
	double denominator = crossProduct(r, s);

	if (numerator == 0 && denominator == 0) {
		// They are colinear

		// Do they touch? (Are any of the points equal?)
		/*if (a == c || a == d || b == c || b == d) {
			return true;
		}*/

		// Do they overlap? (Are all the point differences in either direction the same sign)
		bool xSign = c.x - a.x < 0;
		if (c.x - b.x < 0 != xSign || d.x - a.x < 0 != xSign || d.x - b.x < 0 != xSign) {
			return true;
		}

		bool ySign = c.y - a.y < 0;
		if (c.y - b.y < 0 != ySign || d.y - a.y < 0 != ySign || d.y - b.y < 0 != ySign) {
			return true;
		}

		return false;
	}

	if (denominator == 0) {
		// Lines are parallel
		return false;
	}

	double u = numerator / denominator;
	double t = crossProduct(c - a, s) / denominator;

	return (t > 0) && (t < 1) && (u > 0) && (u < 1);
}

bool Utilities::checkClockwiseness(const std::vector<glm::vec2> & coordinates) {
	double area = 0.0;
	std::size_t size = coordinates.size();
	for (std::size_t i = 0; i < size - 1; ++i) {
		glm::vec2 first = coordinates[i];
		glm::vec2 second = coordinates[i + 1];
		area += (second.x - first.x) * (second.y + first.y);
	}
	return area > 0;
}

glm::vec2 Utilities::findCentroid(const std::vector<glm::vec2> & coordinates) {			//https://en.wikipedia.org/wiki/Centroid
	double area = 0.0;
	double centroidX = 0.0;
	double centroidY = 0.0;
	std::size_t size = coordinates.size();
	for (std::size_t i = 0; i < size - 1; ++i) {
		glm::vec2 first = coordinates[i];
		glm::vec2 second = coordinates[i + 1];

		double newArea = crossProduct(first, second);
		area += newArea;
		centroidX += (first.x + second.x) * newArea;
		centroidY += (first.y + second.y) * newArea;
	}

	area /= 2.0;
	centroidX /= area * 6;
	centroidY /= area * 6;

	return glm::vec2(centroidX, centroidY);
}

bool Utilities::isInsidePolygon(const std::vector<glm::vec2> & coordinates, const glm::vec2 & point) {
	bool inside = false;

	std::size_t size = coordinates.size();
	for (std::size_t i = 0; i < size - 1; ++i) {
		glm::vec2 first = coordinates[i];
		glm::vec2 second = coordinates[i + 1];

		if (doSegmentsIntersect(first, second, point, glm::vec2(point.x, point.y + 1000000))) {
			inside = !inside;
		}
	}

	return inside;
}

glm::vec2 Utilities::findClosestPoint(const glm::vec2 & first, const glm::vec2 & second, const glm::vec2 & point) {
	//https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
	double lengthSquared = pow(second.x - first.x, 2) + pow(second.y - first.y, 2);
	if (lengthSquared == 0) {
		return first;
	}
	double t = dotProduct(point - first, second - first) / lengthSquared;
	if (t <= 0) {
		return first;
	} else if (t >= 1) {
		return second;
	} else {
		return first + (float)t * (second - first);
	}
}

glm::vec2 Utilities::findClosestPoint(const std::vector<glm::vec2> & coordinates, const glm::vec2 & point) {
	glm::vec2 closestPoint;
	double closestDistance = FLT_MAX;

	std::size_t size = coordinates.size();
	for (std::size_t i = 0; i < size - 1; ++i) {
		glm::vec2 first = coordinates[i];
		glm::vec2 second = coordinates[i + 1];

		glm::vec2 localClosest = findClosestPoint(first, second, point);
		double localDistance = distance(localClosest, point);
		if (localDistance < closestDistance) {
			closestPoint = localClosest;
			closestDistance = localDistance;
		}
	}

	return closestPoint;
}

bool Utilities::intersectsSome(const std::vector<glm::vec2> & coordinates, const glm::vec2 & begin, const glm::vec2 & end) {
	std::size_t size = coordinates.size();
	for (std::size_t i = 0; i < size - 1; ++i) {
		glm::vec2 first = coordinates[i];
		glm::vec2 second = coordinates[i + 1];

		if (doSegmentsIntersect(begin, end, first, second)) {
			return true;
		}
	}

	return false;
}