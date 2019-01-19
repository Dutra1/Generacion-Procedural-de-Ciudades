#pragma once
#ifndef RULE
#define RULE
#include "LHS.h"
#include "RHS.h"
#include "Usings.h"

class Rule {
private:
	LHS lhs;
	RHS rhs;
public:
	Rule();
	Rule(const LHS & lhs, const RHS & rhs);

	std::string getSymbol() const;
	double getTotalWeight(ShapeParameters, double lod) const;
	bool isRemembered() const;

	void applyRule(ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables) const;
};

#endif