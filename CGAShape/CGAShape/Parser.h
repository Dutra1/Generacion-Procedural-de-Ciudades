#pragma once
#ifndef PARSER
#define PARSER
#include "Usings.h"
#include "BaseDistribution.h"

class Parser {
public:
	static ShapeCondition parseCondition(const pugi::xml_node & node);
	static ShapeArgument parseArgument(const pugi::xml_node & node);
	
	static std::string parseSymbol(const pugi::xml_node & node);
	static bool parseBool(const pugi::xml_node & node);
	static double parseDouble(const pugi::xml_node & node);
	static Axis parseAxis(const pugi::xml_node & node);
	static Component parseComponent(const pugi::xml_node & node);
	static BaseDistribution parseDistribution(const pugi::xml_node & node);
};

#endif

