#pragma once
#ifndef MAP
#define MAP
#include "Building.h"
#include "Street.h"
#include "RulePool.h"
#include "Drawable.h"
#include "ProgramHolder.h"
#include "MaterialHolder.h"
#include "ShapeInfoHolder.h"

class Map {
private:
	ShapeArgument buildingHeight;
	std::vector<Street> streets;
	std::vector<Building> buildings;
	std::unique_ptr<Drawable> drawable;
public:
	Map();
	void setBuildingHeight(const ShapeArgument & function);
	double getBuildingHeight(ShapeParameters);

	void addBuilding(const Building & building);
	void growAll(const RulePool & rulepool, double lod);

	void addStreet(const std::vector<glm::vec2> & coords, double width);

	void parseMap(const std::string & mapFIle, bool rectangleize);

	void buildDrawable(GLuint program, const ShapeInfoHolder & shapeInfoHolder);
	std::string toOBJ(const std::string & materialFile) const;
	
	static bool prepareDraw(ProgramHolder & programHolder);
	void draw(MaterialHolder & materialHolder, GLuint uniColor) const;
	void quit();
};

#endif


