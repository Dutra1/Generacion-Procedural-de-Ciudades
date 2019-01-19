#pragma once
#ifndef FLOORPLAN
#define FLOORPLAN

//Simple polygon for now
class Floorplan {
private:
	glm::vec2 position, size;
	std::vector<glm::vec2> floor;
public:
	Floorplan(const std::vector<glm::vec2> & coordinates);	//Extra point to close the loop? It comes from osm, and is repeated

	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;
	std::vector<glm::vec2> getFloor() const;
};

#endif


