#pragma once
#ifndef LEFT_HAND_SIDE
#define LEFT_HAND_SIDE
#include "ShapeTree.h"
#include "Usings.h"

class LHS {
private:
	std::string symbol;
	ShapeCondition matches;
	ShapeArgument weight;

	bool remembered;
	double lod;
public:
	LHS(const char* parameter);
	LHS(const std::string & symbol,
		double lod = 0,
		const ShapeCondition & matches = LambdaShape(true),
		const ShapeArgument & weight = LambdaShape(1),
		bool isRemembered = true
	);

	void setSymbol(const std::string & symbol);
	void setLod(double lod);
	void setMatchesFunction(const ShapeCondition & matches);
	void setWeightFunction(const ShapeArgument & weight);
	void setIsRemembered(bool isRemembered);

	std::string getSymbol() const;
	double getTotalWeight(ShapeParameters, double lod) const;
	bool isRemembered() const;
};

#endif