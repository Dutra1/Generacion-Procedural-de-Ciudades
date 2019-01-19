#include "stdafx.h"
#include "BaseDistribution.h"

BaseDistribution::BaseDistribution()															: type(DistributionType::NOTHING) {}

BaseDistribution::BaseDistribution(const std::uniform_int_distribution<int> & uniform_int)		: type(DistributionType::UNIFORM_INT), uniform_int(uniform_int) {}

BaseDistribution::BaseDistribution(const std::uniform_real_distribution<double> & uniform_real) : type(DistributionType::UNIFORM_REAL), uniform_real(uniform_real) {}

BaseDistribution::BaseDistribution(const std::normal_distribution<double> & normal)				: type(DistributionType::NORMAL), normal(normal) {}

BaseDistribution::BaseDistribution(const std::poisson_distribution<int> & poisson)				: type(DistributionType::POISSON), poisson(poisson) {}

BaseDistribution::BaseDistribution(const std::exponential_distribution<double> & exponential)	: type(DistributionType::EXPONENTIAL), exponential(exponential) {}

double BaseDistribution::use(std::default_random_engine & generator) const {
	switch (type) {
	case DistributionType::NOTHING:
		return 0.0;
		break;
	case DistributionType::UNIFORM_INT:
		return uniform_int(generator);
		break;
	case DistributionType::UNIFORM_REAL:
		return uniform_real(generator);
		break;
	case DistributionType::NORMAL:
		return normal(generator);
		break;
	case DistributionType::POISSON:
		return poisson(generator);
		break;
	case DistributionType::EXPONENTIAL:
		return exponential(generator);
		break;
	default:
		return 0.0;
		break;
	}
}
