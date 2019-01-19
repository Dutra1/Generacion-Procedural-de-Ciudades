#pragma once
#ifndef UTILITIES
#define UTILITIES

class Utilities {
public:
	static double dotProduct(const glm::vec2 & p1, const glm::vec2 & p2);
	static double crossProduct(const glm::vec2 & p1, const glm::vec2 & p2);
	static float hypotenuse(float a, float b);
	static double distance(const glm::vec2 & a, const glm::vec2 & b);
	static bool doSegmentsIntersect(const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & d);
	static bool checkClockwiseness(const std::vector<glm::vec2> & coordinates);
	static glm::vec2 findCentroid(const std::vector<glm::vec2> & coordinates);
	static bool isInsidePolygon(const std::vector<glm::vec2> & coordinates, const glm::vec2 & point);
	static glm::vec2 findClosestPoint(const glm::vec2 & first, const glm::vec2 & second, const glm::vec2 & point);
	static glm::vec2 findClosestPoint(const std::vector<glm::vec2> & coordinates, const glm::vec2 & point);
	static bool intersectsSome(const std::vector<glm::vec2> & coordinates, const glm::vec2 & begin, const glm::vec2 & end);
};

#endif

