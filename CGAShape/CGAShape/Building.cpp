#include "stdafx.h"
#include "Building.h"
#include "ShaderRepository.h"

Building::Building(const ShapeTree & tree, std::size_t seed) : tree(tree), generator(std::default_random_engine(seed)) {}

void Building::grow(const RulePool & rulepool, double lod) {
	rulepool.grow(tree, lod, generator, variables, rememberedRules);
}

const ShapeTree & Building::getTree() const {
	return tree;
}

std::default_random_engine & Building::getGenerator() {
	return generator;
}