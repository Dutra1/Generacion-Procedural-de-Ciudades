#include "stdafx.h"
#include "RulePool.h"
#include "WeightedSet.h"
#include "Parser.h"

void RulePool::addRule(const LHS & lhs, const RHS & rhs) {
	addRule(Rule(lhs, rhs));
}

void RulePool::addRule(const Rule & rule) {
	rules.emplace(rule.getSymbol(), rule);
}

void RulePool::grow(ShapeTree & tree, double lod, std::default_random_engine & generator, std::map<std::string, double> & variables, std::multimap<std::string, Rule> & rememberedRules) const {
	if (tree.isLeaf()) {
		Shape & shape = tree.getShape();
		std::string symbol = shape.getSymbol();
		std::size_t totalRules = rules.count(symbol);

		if (totalRules > 0) {
			Rule chosenRule;
			bool foundRule = false;

			//Check remembered rules
			auto rememberedBounds = rememberedRules.equal_range(symbol);
			for (auto it = rememberedBounds.first; it != rememberedBounds.second; ++it) {
				Rule rule = it->second;
				if (rule.getTotalWeight(ShapePassParameters, lod) > 0) {
					chosenRule = rule;
					foundRule = true;
					break;
				}
			}

			//Choose a new rule
			if (!foundRule) {
				WeightedSet<Rule> weightedSet;
				double totalWeight = 0.0;
				auto bounds = rules.equal_range(symbol);
				for (auto it = bounds.first; it != bounds.second; ++it) {
					Rule rule = it->second;
					double weight = rule.getTotalWeight(ShapePassParameters, lod);
					if (weight > 0) {
						totalWeight += weight;
						weightedSet.add(weight, rule);
					}

				}

				if (totalWeight > 0) {
					chosenRule = weightedSet.getRandom(generator);
					foundRule = true;
					if (chosenRule.isRemembered()) {
						rememberedRules.emplace(symbol, chosenRule);
					}
				}
			}

			//Apply Rule
			if (foundRule) {
				chosenRule.applyRule(tree, generator, variables);
				for (std::size_t i = 0; i < tree.getChildrenCount(); ++i) {
					grow(tree[i], lod, generator, variables, rememberedRules);
				}
			}
		}
	}
}

LHS parseLHS(const pugi::xml_node & node) {
	std::string symbol =									  Parser::parseSymbol(node.child("symbol"));
	bool remembered =			node.child("remembered")	? Parser::parseBool(node.child("remembered"))					: true;
	ShapeCondition matches =	node.child("matches")		? Parser::parseCondition(node.child("matches").first_child())	: LambdaShape(true);
	ShapeArgument weight =		node.child("weight")		? Parser::parseArgument(node.child("weight").first_child())		: LambdaShape(1);
	double lod =				node.child("lod")			? Parser::parseDouble(node.child("lod"))						: 0.0;

	return LHS(symbol, remembered, matches, weight, lod);
}

RHS parseRHS(const pugi::xml_node & node) {
	RHS rhs = RHS();

	for (auto simpleRule : node.children()) {
		std::string name = simpleRule.name();
		auto first = simpleRule.first_child();
		auto second = first ? first.next_sibling() : pugi::xml_node();
		auto third = second ? second.next_sibling() : pugi::xml_node();
		auto fourth = third ? third.next_sibling() : pugi::xml_node();


		if (name == "translateXYZ") {
			ShapeArgument x = Parser::parseArgument(first);
			ShapeArgument y = Parser::parseArgument(second);
			ShapeArgument z = Parser::parseArgument(third);
			rhs.translate(x, y, z);
		} else if (name == "translateAxis") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument value = Parser::parseArgument(second);
			rhs.translate(axis, value);
		} else if (name == "relativeTranslateXYZ") {
			ShapeArgument x = Parser::parseArgument(first);
			ShapeArgument y = Parser::parseArgument(second);
			ShapeArgument z = Parser::parseArgument(third);
			rhs.relativeTranslate(x, y, z);
		} else if (name == "relativeTranslateAxis") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument value = Parser::parseArgument(second);
			rhs.relativeTranslate(axis, value);


		} else if (name == "rotate") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument angle = Parser::parseArgument(second);
			if (third) {
				rhs.rotate(axis, angle, Parser::parseBool(third));
			} else {
				rhs.rotate(axis, angle);
			}


		} else if (name == "resizeXYZ") {
			ShapeArgument x = Parser::parseArgument(first);
			ShapeArgument y = Parser::parseArgument(second);
			ShapeArgument z = Parser::parseArgument(third);
			if (fourth) {
				rhs.resize(x, y, z, Parser::parseBool(fourth));
			} else {
				rhs.resize(x, y, z);
			}
		} else if (name == "resizeAxis") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument value = Parser::parseArgument(second);
			if (third) {
				rhs.resize(axis, value, Parser::parseBool(third));
			} else {
				rhs.resize(axis, value);
			}


		} else if (name == "scaleXYZ") {
			ShapeArgument x = Parser::parseArgument(first);
			ShapeArgument y = Parser::parseArgument(second);
			ShapeArgument z = Parser::parseArgument(third);
			if (fourth) {
				rhs.scale(x, y, z, Parser::parseBool(fourth));
			} else {
				rhs.scale(x, y, z);
			}
		} else if (name == "scaleAxis") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument value = Parser::parseArgument(second);
			if (third) {
				rhs.scale(axis, value, Parser::parseBool(third));
			} else {
				rhs.scale(axis, value);
			}
		} else if (name == "scale") {
			ShapeArgument value = Parser::parseArgument(second);
			if (second) {
				rhs.scale(value, Parser::parseBool(second));
			} else {
				rhs.scale(value);
			}
			

		} else if (name == "mirror") {
			Axis axis = Parser::parseAxis(first);
			rhs.mirror(axis);
		} else if (name == "flatten") {
			Axis axis = Parser::parseAxis(first);
			rhs.flatten(axis);


		} else if (name == "viewComponent") {
			Component component = Parser::parseComponent(first);
			rhs.view(component);
		} else if (name == "view") {
			Axis axis1 = Parser::parseAxis(first);
			bool bool1 = Parser::parseBool(second);
			Axis axis2 = Parser::parseAxis(third);
			bool bool2 = Parser::parseBool(fourth);
			rhs.view(axis1, bool1, axis2, bool2);


		} else if (name == "instantiate") {
			std::string symbol = Parser::parseSymbol(first);
			rhs.instantiate(symbol);
		} else if (name == "split") {
			Axis axis = Parser::parseAxis(first);
			std::vector<ArgumentRatio> ratios;
			for (auto ratio : second.children()) {
				auto ratioFirst = ratio.first_child();
				auto ratioSecond = ratioFirst.next_sibling();
				auto ratioThird = ratioSecond.next_sibling();
				ShapeArgument argument = Parser::parseArgument(ratioFirst);
				bool isAbsolute = Parser::parseBool(ratioSecond);
				std::string symbol = Parser::parseSymbol(ratioThird);
				ratios.push_back({ argument, isAbsolute, symbol });
			}
			rhs.split(axis, ratios);
		} else if (name == "repeat") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument size = Parser::parseArgument(second);
			std::string symbol = Parser::parseSymbol(third);
			rhs.repeat(axis, size, symbol);
		} else if (name == "repeatRand") {
			Axis axis = Parser::parseAxis(first);
			ShapeArgument size = Parser::parseArgument(second);
			std::string symbol = Parser::parseSymbol(third);
			BaseDistribution rand = Parser::parseDistribution(fourth);
			rhs.repeat(axis, size, symbol, rand);
		} else if (name == "component") {
			Component component = Parser::parseComponent(first);
			std::string symbol = Parser::parseSymbol(second);
			rhs.component(component, symbol);
		} else if (name == "inheritFloorPlan") {
			rhs.inheritFloorPlan();


		} else if (name == "clearAndSetVariable") {
			std::string variable = Parser::parseSymbol(first);
			ShapeArgument value = Parser::parseArgument(second);
			rhs.clearAndSetVariable(variable, value);
		} else if (name == "setVariable") {
			std::string variable = Parser::parseSymbol(first);
			ShapeArgument value = Parser::parseArgument(second);
			rhs.setVariable(variable, value);
		} else if (name == "clearVariable") {
			std::string variable = Parser::parseSymbol(first);
			rhs.clearVariable(variable);


		} else if (name == "push") {
			rhs.push();
		} else if (name == "pop") {
			rhs.pop();
		}
	}

	return rhs;
}

Rule parseRule(const pugi::xml_node & node) {
	LHS lhs = parseLHS(node.child("lhs"));
	RHS rhs = parseRHS(node.child("rhs"));

	return Rule(lhs, rhs);
}

void RulePool::parseRulepool(const std::string & rulepoolFile) {
	pugi::xml_document doc;
	doc.load_file(rulepoolFile.c_str());

	auto root = doc.child("rulepool");

	for (auto rule = root.child("rule"); rule; rule = rule.next_sibling("rule")) {
		addRule(parseRule(rule));
	}
}