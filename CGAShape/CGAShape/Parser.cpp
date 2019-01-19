#include "stdafx.h"
#include "Parser.h"
#include "Axis.h"
#include "Component.h"

ShapeCondition Parser::parseCondition(const pugi::xml_node & node) {
	ShapeCondition condition = LambdaShape(false);
	std::string operation = node.name();

	/* Boolean Operations */
	if (operation == "and") {
		condition = LambdaShape(true);
		for (auto child : node.children()) {
			ShapeCondition childCondition = parseCondition(child);
			condition = LambdaShape(condition(ShapePassParameters) && childCondition(ShapePassParameters));
		}
	} else if (operation == "or") {
		condition = LambdaShape(false);
		for (auto child : node.children()) {
			ShapeCondition childCondition = parseCondition(child);
			condition = LambdaShape(condition(ShapePassParameters) || childCondition(ShapePassParameters));
		}
	} else if (operation == "not") {
		ShapeCondition childCondition = parseCondition(node.first_child());
		condition = LambdaShape(!condition(ShapePassParameters));

	/* Comparisons */
	} else if (operation == "eq" || operation == "ne" || operation == "gt" || operation == "lt" || operation == "ge" || operation == "le") {
		auto first = node.first_child();
		auto second = first.next_sibling();
		ShapeArgument firstArgument = parseArgument(first);
		ShapeArgument secondArgument = parseArgument(second);
		if (operation == "eq") {
			condition = LambdaShape(firstArgument(ShapePassParameters) == secondArgument(ShapePassParameters));
		} else if (operation == "ne") {
			condition = LambdaShape(firstArgument(ShapePassParameters) != secondArgument(ShapePassParameters));
		} else if (operation == "gt") {
			condition = LambdaShape(firstArgument(ShapePassParameters) > secondArgument(ShapePassParameters));
		} else if (operation == "lt") {
			condition = LambdaShape(firstArgument(ShapePassParameters) < secondArgument(ShapePassParameters));
		} else if (operation == "ge") {
			condition = LambdaShape(firstArgument(ShapePassParameters) >= secondArgument(ShapePassParameters));
		} else if (operation == "le") {
			condition = LambdaShape(firstArgument(ShapePassParameters) <= secondArgument(ShapePassParameters));
		}

	/* Basic truth values */
	} else if (operation == "true") {
		condition = LambdaShape(true);
	} else if (operation == "false") {
		condition = LambdaShape(false);
	}

	return condition;
}

ShapeArgument Parser::parseArgument(const pugi::xml_node & node) {
	ShapeArgument argument = LambdaShape(0);
	std::string operation = node.name();

	/* Functions */
	if (operation == "add") {
		argument = LambdaShape(0);
		for (auto child : node.children()) {
			ShapeArgument childArgument = parseArgument(child);
			argument = LambdaShape(argument(ShapePassParameters) + childArgument(ShapePassParameters));
		}
	} else if (operation == "mul") {
		argument = LambdaShape(1);
		for (auto child : node.children()) {
			ShapeArgument childArgument = parseArgument(child);
			argument = LambdaShape(argument(ShapePassParameters) * childArgument(ShapePassParameters));
		}
	} else if (operation == "max") {
		argument = LambdaShape(DBL_MIN);
		for (auto child : node.children()) {
			ShapeArgument childArgument = parseArgument(child);
			argument = LambdaShape(std::max(argument(ShapePassParameters), childArgument(ShapePassParameters)));
		}
	} else if (operation == "min") {
		argument = LambdaShape(DBL_MAX);
		for (auto child : node.children()) {
			ShapeArgument childArgument = parseArgument(child);
			argument = LambdaShape(std::min(argument(ShapePassParameters), childArgument(ShapePassParameters)));
		}
	} else if (operation == "sub" || operation == "div" || operation == "mod" || operation == "pow") {
		auto first = node.first_child();
		auto second = first.next_sibling();
		ShapeArgument firstArgument = parseArgument(first);
		ShapeArgument secondArgument = parseArgument(second);
		if (operation == "sub") {
			argument = LambdaShape(firstArgument(ShapePassParameters) - secondArgument(ShapePassParameters));
		} else if (operation == "div") {
			argument = LambdaShape(firstArgument(ShapePassParameters) / secondArgument(ShapePassParameters));
		} else if (operation == "mod") {
			argument = LambdaShape(fmod(firstArgument(ShapePassParameters), secondArgument(ShapePassParameters)));
		} else if (operation == "pow") {
			argument = LambdaShape(pow(firstArgument(ShapePassParameters), secondArgument(ShapePassParameters)));
		}
	} else if (operation == "neg" || operation == "abs" || operation == "sin" || operation == "cos" || operation == "tan" || operation == "sqrt") {
		ShapeCondition childArgument = parseArgument(node.first_child());
		if (operation == "neg") {
			argument = LambdaShape(-childArgument(ShapePassParameters));
		} else if (operation == "abs") {
			argument = LambdaShape(abs(childArgument(ShapePassParameters)));
		} else if (operation == "sin") {
			argument = LambdaShape(sin(childArgument(ShapePassParameters)));
		} else if (operation == "cos") {
			argument = LambdaShape(cos(childArgument(ShapePassParameters)));
		} else if (operation == "tan") {
			argument = LambdaShape(tan(childArgument(ShapePassParameters)));
		} else if (operation == "sqrt") {
			argument = LambdaShape(sqrt(childArgument(ShapePassParameters)));
		}

	/* Basic values */
	} else if (operation == "num") {
		double value = node.attribute("value").as_double();
		argument = LambdaShape(value);
	} else if (operation == "var") {
		std::string variable = node.attribute("name").as_string();
		argument = LambdaShape(variables.count(variable) > 0 ? variables.at(variable) : 0);
	} else if (operation == "pos") {
		Axis axis = parseAxis(node);
		argument = LambdaShape(shape.getPosition(axis));
	} else if (operation == "size") {
		Axis axis = parseAxis(node);
		argument = LambdaShape(shape.getSize(axis));
	} else if (operation == "rand") {
		BaseDistribution distribution = parseDistribution(node);
		argument = LambdaShape(distribution.use(generator));
	}

	return argument;
}

std::string Parser::parseSymbol(const pugi::xml_node & node) {
	return node.attribute("value").as_string();
}

bool Parser::parseBool(const pugi::xml_node & node) {
	return node.attribute("value").as_bool();
}

double Parser::parseDouble(const pugi::xml_node & node) {
	return node.attribute("value").as_double();
}

Axis Parser::parseAxis(const pugi::xml_node & node) {
	std::string axisString = node.attribute("axis").as_string();
	return axisString == "X" ? Axis::X : axisString == "Y" ? Axis::Y : Axis::Z;
}

Component Parser::parseComponent(const pugi::xml_node & node) {
	std::string componentString = node.attribute("component").as_string();

	if (componentString == "LEFT") {
		return Component::LEFT;
	} else if (componentString == "RIGHT") {
		return Component::RIGHT;
	} else if (componentString == "FRONT") {
		return Component::FRONT;
	} else if (componentString == "BACK") {
		return Component::BACK;
	} else if (componentString == "TOP") {
		return Component::TOP;
	} else if (componentString == "BOTTOM") {
		return Component::BOTTOM;
	} else if (componentString == "SLOPE") {
		return Component::SLOPE;
	} else if (componentString == "MIRRORED_LEFT_RIGHT") {
		return Component::MIRRORED_LEFT_RIGHT;
	} else if (componentString == "FLOOR") {
		return Component::FLOOR;
	} else if (componentString == "CEILING") {
		return Component::CEILING;
	} else if (componentString == "SIDES") {
		return Component::SIDES;
	} else if (componentString == "RING") {
		return Component::RING;
	} else {
		return Component::POLYGON_WALLS;
	}
}

BaseDistribution Parser::parseDistribution(const pugi::xml_node & node) {
	std::string type = node.attribute("type").as_string();
	if (type == "int") {
		int begin = node.attribute("begin").as_int();
		int end = node.attribute("end").as_int();
		std::uniform_int_distribution<> distribution = std::uniform_int_distribution<int>(begin, end);
		return BaseDistribution(distribution);
	} else if (type == "double") {
		double begin = node.attribute("begin").as_double();
		double end = node.attribute("end").as_double();
		std::uniform_real_distribution<> distribution = std::uniform_real_distribution<double>(begin, end);
		return BaseDistribution(distribution);
	} else if (type == "normal") {
		double mean = node.attribute("mean").as_double();
		double deviation = node.attribute("deviation").as_double();
		std::normal_distribution<> distribution = std::normal_distribution<double>(mean, deviation);
		return BaseDistribution(distribution);
	} else if (type == "poisson") {
		int mean = node.attribute("mean").as_int();
		std::poisson_distribution<> distribution = std::poisson_distribution<int>(mean);
		return BaseDistribution(distribution);
	} else if (type == "exp") {
		double lambda = node.attribute("lambda").as_double();
		std::exponential_distribution<> distribution = std::exponential_distribution<double>(lambda);
		return BaseDistribution(distribution);
	} else {
		return BaseDistribution(std::uniform_int_distribution<int>(0, 0));
	}
}
