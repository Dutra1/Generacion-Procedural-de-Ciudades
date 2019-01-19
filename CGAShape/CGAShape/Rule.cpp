#include "stdafx.h"
#include "Rule.h"

Rule::Rule(): lhs(LHS("INVALID")), rhs(RHS()) {}

Rule::Rule(const LHS & lhs, const RHS & rhs): lhs(lhs), rhs(rhs) {}

std::string Rule::getSymbol() const {
	return lhs.getSymbol();
}

double Rule::getTotalWeight(ShapeParameters, double lod) const {
	return lhs.getTotalWeight(ShapePassParameters, lod);
}

bool Rule::isRemembered() const {
	return lhs.isRemembered();
}

void Rule::applyRule(ShapeTree & tree, std::default_random_engine & generator, std::map<std::string, double> & variables) const {
	rhs.applyRule(tree, generator, variables);
}