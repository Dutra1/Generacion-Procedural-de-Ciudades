#include "stdafx.h"
#include "Config.h"
#include "Parser.h"

std::string Config::windowName = "Precedural Cities";
std::string Config::materialsFile = "configs/Materials.xml";
std::string Config::shapeInfoFile = "configs/ShapeInfo.xml";
std::string Config::rulepoolFile = "configs/Rulepool.xml";
std::string Config::mapToParseFile = "maps/map4.osm";
std::string Config::modelFile = "models/map";
std::string Config::buildingDefaultSymbol = "Building";
std::string Config::polygonizedBuildingDefaultSymbol = "Building Polygonized";

bool Config::rectanglize = true;

int Config::screenWidth = 1280;
int Config::screenHeight = 720;

double Config::lod = 0.0;
double Config::streetWidth = 10.0;
double Config::cameraSpeed = 100.0;

glm::vec3 Config::cameraPosition = glm::vec3(-50.0f, 100.0f, -250.0f);
glm::vec3 Config::sunDirection = glm::vec3(-1.0f, 5.0f, -3.0f);

ShapeArgument Config::buildingHeight = LambdaShape(100);

void Config::parseConfig(const std::string & configFile) {
	pugi::xml_document doc;
	doc.load_file(configFile.c_str());

	auto root = doc.child("config");

	windowName =		root.child("windowName").attribute("value").as_string();
	materialsFile =		root.child("materialsFile").attribute("value").as_string();
	shapeInfoFile =		root.child("shapeInfoFile").attribute("value").as_string();
	rulepoolFile =		root.child("rulepoolFile").attribute("value").as_string();
	mapToParseFile =	root.child("mapToParseFile").attribute("value").as_string();
	modelFile =			root.child("modelFile").attribute("value").as_string();
	buildingDefaultSymbol =				root.child("buildingDefaultSymbol").attribute("value").as_string();
	polygonizedBuildingDefaultSymbol =	root.child("polygonizedBuildingDefaultSymbol").attribute("value").as_string();

	rectanglize = root.child("rectanglize").attribute("value").as_bool();

	screenWidth =	root.child("screenWidth").attribute("value").as_int();
	screenHeight =	root.child("screenHeight").attribute("value").as_int();

	lod =			root.child("lod").attribute("value").as_double();
	streetWidth =	root.child("streetWidth").attribute("value").as_double();
	cameraSpeed =	root.child("cameraSpeed").attribute("value").as_double();

	auto cameraPositionNode = root.child("cameraPosition");
	cameraPosition = glm::vec3(cameraPositionNode.attribute("x").as_float(), cameraPositionNode.attribute("y").as_float(), cameraPositionNode.attribute("z").as_float());

	auto sunDirectionNode = root.child("sunDirection");
	sunDirection = glm::vec3(sunDirectionNode.attribute("x").as_float(), sunDirectionNode.attribute("y").as_float(), sunDirectionNode.attribute("z").as_float());

	buildingHeight = Parser::parseArgument(root.child("buildingHeight").first_child());
}