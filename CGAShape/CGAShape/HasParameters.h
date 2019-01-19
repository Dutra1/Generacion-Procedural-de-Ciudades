#pragma once
#ifndef HAS_PARAMETERS
#define HAS_PARAMETERS

class HasParameters {
protected:
	std::map<std::string, double> parameters;
public:
	bool hasParameter(const std::string & key) const;
	void addParameter(const std::string & key, double value);
	double getParameter(const std::string & key) const;
	std::size_t parameterCount() const;
};

#endif
