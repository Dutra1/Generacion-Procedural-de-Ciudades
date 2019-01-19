#include "stdafx.h"
#include "LHS.h"

LHS::LHS(const char* symbol) : LHS(	symbol, 0, LambdaShape(true), LambdaShape(1), true) {};

LHS::LHS(const std::string & symbol, double lod, const ShapeCondition & matches, const ShapeArgument & weight, bool isRemembered):
	symbol(symbol), lod(lod), matches(matches), weight(weight), remembered(isRemembered) {}

void LHS::setSymbol(const std::string & symbol) {
	this->symbol = symbol;
}

void LHS::setLod(double lod) {
	this->lod = lod;
}

void LHS::setMatchesFunction(const ShapeCondition & matches) {
	this->matches = matches;
}

void LHS::setWeightFunction(const ShapeArgument & weight) {
	this->weight = weight;
}

void LHS::setIsRemembered(bool isRemembered) {
	remembered = isRemembered;
}

std::string LHS::getSymbol() const {
	return symbol;
}

double LHS::getTotalWeight(ShapeParameters, double lod) const {
	if (shape.getSymbol() == symbol && matches(shape, generator, variables) && (this->lod <= lod || lod == 0)) {
		return weight(shape, generator, variables);
	} else {
		return 0.0;
	}
}

bool LHS::isRemembered() const {
	return remembered;
}