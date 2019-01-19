#pragma once
#ifndef BUILDING_DRAWER
#define BUIDLING_DRAWER
#include "ShapeTree.h"
#include "RulePool.h"

class Building {
private:
	ShapeTree tree;

	std::default_random_engine generator;
	std::map<std::string, double> variables;
	std::multimap<std::string, Rule> rememberedRules;
public:
	Building(const ShapeTree & tree, std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count());
	void grow(const RulePool & rulepool, double lod = 0);

	const ShapeTree & getTree() const;
	std::default_random_engine & getGenerator();
};

#endif
