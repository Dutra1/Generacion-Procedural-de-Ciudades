#pragma once
#ifndef CONFIG
#define CONFIG
#include "Usings.h"

class Config {
public:
	static std::string windowName;
	static std::string materialsFile;
	static std::string shapeInfoFile;
	static std::string rulepoolFile;
	static std::string mapToParseFile;
	static std::string modelFile;
	static std::string buildingDefaultSymbol;
	static std::string polygonizedBuildingDefaultSymbol;

	static bool rectanglize;

	static int screenWidth;
	static int screenHeight;

	static double lod;
	static double streetWidth;
	static double cameraSpeed;

	static glm::vec3 cameraPosition;
	static glm::vec3 sunDirection;

	static ShapeArgument buildingHeight;

	static void parseConfig(const std::string & configFile);
};

#endif

