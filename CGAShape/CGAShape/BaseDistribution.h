#pragma once
#ifndef BASE_DISTRIBUTION
#define BASE_DISTRIBUTION

enum class DistributionType {
	NOTHING,
	UNIFORM_INT,
	UNIFORM_REAL,
	POISSON,
	EXPONENTIAL,
	NORMAL
};

class BaseDistribution {
private:
	DistributionType type;
	union {
		std::uniform_int_distribution<int> uniform_int;
		std::uniform_real_distribution<double> uniform_real;
		mutable std::normal_distribution<double> normal;		/* Apparently this keeps a state */
		std::poisson_distribution<int> poisson;
		std::exponential_distribution<double> exponential;
	};
public:
	BaseDistribution();
	BaseDistribution(const std::uniform_int_distribution<int> & uniform_int);
	BaseDistribution(const std::uniform_real_distribution<double> & uniform_real);
	BaseDistribution(const std::normal_distribution<double> & normal);
	BaseDistribution(const std::poisson_distribution<int> & poisson);
	BaseDistribution(const std::exponential_distribution<double> & exponential);

	double use(std::default_random_engine & generator) const;
};

#endif