#include "stdafx.h"
#include "HasParameters.h"

bool HasParameters::hasParameter(const std::string & key) const {
	return parameters.count(key) > 0;
}

void HasParameters::addParameter(const std::string & key, double value) {
	parameters[key] = value;
}

double HasParameters::getParameter(const std::string & key) const {
	return parameters.at(key);
}

std::size_t HasParameters::parameterCount() const {
	return parameters.size();
}