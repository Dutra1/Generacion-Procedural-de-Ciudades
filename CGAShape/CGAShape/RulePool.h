#pragma once
#ifndef RULE_POOL
#define RULE_POOL
#include "Rule.h"

class RulePool {
private:
	std::multimap<std::string, Rule> rules;
public:
	void addRule(const LHS & lhs, const RHS & rhs);
	void addRule(const Rule & rule);
	void grow(ShapeTree & tree, double lod, std::default_random_engine & generator, std::map<std::string, double> & variables, std::multimap<std::string, Rule> & rememberedRules) const;

	void parseRulepool(const std::string & rulepoolFile);
};

#endif

