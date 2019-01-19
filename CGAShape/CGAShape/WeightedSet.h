#pragma once
#ifndef WEIGHTED_SET
#define WEIGHTED_SET

template <class P> class WeightedSet {
private:
	std::vector<std::pair<double, P>> set;
	double totalWeight;
	std::uniform_real_distribution<double> distribution;
public:
	WeightedSet() : totalWeight(0), distribution(std::uniform_real_distribution<double>(0, 0)) {}

	void add(double weight, const P & item) {
		if (weight > 0.0) {
			set.push_back(std::make_pair(weight, item));
			totalWeight += weight;
			distribution = std::uniform_real_distribution<double>(0, totalWeight);
		}
	}

	bool isEmpty() const {
		return set.empty();
	}

	const P & getRandom(std::default_random_engine & generator) const {
		double random = distribution(generator);
		for (auto const & possibleValue : set) {
			random -= possibleValue.first;
			if (random < 0) {
				return possibleValue.second;
			}
		}
		throw std::logic_error("Weighted Set has no items");
	}
};

#endif