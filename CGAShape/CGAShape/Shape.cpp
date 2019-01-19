#include "stdafx.h"
#include "Shape.h"

Shape::Shape(const std::string & symbol) : symbol(symbol) {}

void Shape::setSymbol(const std::string & symbol) {
	this->symbol = symbol;
}

std::string Shape::getSymbol() const {
	return symbol;
}

void Shape::applyFloorplan(const Floorplan & floorplan) {
	floor = floorplan.getFloor();
}

void Shape::copyFloor(const Shape & shapeWithFloor) {
	this->floor = shapeWithFloor.floor;
}

bool Shape::hasFloorplan() const {
	return floor.size() > 0;
}

std::size_t Shape::getFloorPointCount() const {
	return floor.size();
}

glm::vec2 Shape::getFloorPoint(std::size_t index) const {
	return floor.at(index);
}

std::vector<Shape> Shape::split(Axis axis, const std::vector<Ratio> & ratios) {
	std::vector<Shape> result;

	double totalRelative = 0.0f;
	double totalAbsolute = 0.0f;

	for (std::size_t i = 0; i < ratios.size(); ++i) {
		Ratio ratio = ratios[i];
		if (ratio.isAbsolute) {
			totalAbsolute += ratio.value;
		} else {
			totalRelative += ratio.value;
		}
	}

	double axisSize = getSize(axis);
	bool useAbsolutes = totalAbsolute <= abs(axisSize);

	double relativePoint;
	if (useAbsolutes) {
		relativePoint = axisSize > 0 ? (axisSize - totalAbsolute) / totalRelative : (axisSize + totalAbsolute) / totalRelative;
	} else {
		relativePoint = axisSize / totalRelative;
	}

	double movement = 0.0f;
	for (std::size_t i = 0; i < ratios.size(); ++i) {
		Ratio ratio = ratios[i];
		double size;
		if (ratio.isAbsolute) {
			if (axisSize > 0.0f) {
				size = ratio.value;
			} else {
				size = -ratio.value;
			}
		} else {
			size = ratio.value * relativePoint;
		}

		if (useAbsolutes || !ratio.isAbsolute) {
			Shape newShape = Shape();
			newShape.setSymbol(ratio.symbol);
			newShape.resize(getSize());
			newShape.resize(axis, size);
			newShape.translate(axis, movement);
			result.push_back(newShape);

			movement += size;
		}
	}

	return result;
}

std::vector<Shape> Shape::repeat(Axis axis, float size, const std::string & symbol) {

	std::size_t times = static_cast <std::size_t> (std::ceil(getSize(axis) / size));
	return split(axis, std::vector<Ratio>(times, { 1, false, symbol }));
};

std::vector<Shape> Shape::repeat(Axis axis, float size, const std::string & symbol, const BaseDistribution & distribution, std::default_random_engine & generator) {
	std::size_t times = static_cast <std::size_t> (std::ceil(getSize(axis) / size));

	std::vector<Ratio> ratios;
	for (std::size_t i = 0; i < times; ++i) {
		ratios.push_back({ distribution.use(generator), false, symbol });
	}
	return split(axis, ratios);
};

std::vector<Shape> Shape::component(Component component, std::string symbol) {
	std::vector<Shape> result;

	Shape childShape = Shape();
	childShape.resize(getSize());
	childShape.setSymbol(symbol);

	switch (component) {
	case Component::FRONT:
	case Component::BACK:
	case Component::LEFT:
	case Component::RIGHT:
	case Component::TOP:
	case Component::BOTTOM:
		childShape.view(component);
		childShape.flatten(Axis::Z);
		result.push_back(childShape);
		break;
	case Component::SLOPE: {
		childShape.rotate(Axis::X, atan2(childShape.getSize(Axis::Z), childShape.getSize(Axis::Y)) * 180.0 / PI, false);
		childShape.resize(Axis::Y, sqrt(pow(childShape.getSize(Axis::Y), 2) + pow(childShape.getSize(Axis::Z), 2)));
		childShape.flatten(Axis::Z);
		result.push_back(childShape);
		break;
	}
	case Component::MIRRORED_LEFT_RIGHT: {
		Shape face = Shape(childShape);
		face.view(Axis::X, true, Axis::Y, true);
		face.flatten(Axis::Z);
		result.push_back(face);

		face = Shape(childShape);
		face.view(Axis::X, false, Axis::Y, true);
		face.flatten(Axis::Z);
		face.view(Axis::Z, true, Axis::Y, true);
		result.push_back(face);
		break;
	}
	case Component::FLOOR:
		childShape.view(Axis::Y, false, Axis::Z, false);
		childShape.flatten(Axis::Z);
		childShape.rotate(Axis::X, 180, true);
		result.push_back(childShape);
		break;
	case Component::CEILING:
		childShape.view(Axis::Y, true, Axis::Z, false);
		childShape.flatten(Axis::Z);
		childShape.rotate(Axis::X, 180, true);
		result.push_back(childShape);
		break;
	case Component::SIDES:
		for (std::size_t i = 0; i < 4; ++i) {
			Shape rotated = Shape(childShape);
			rotated.flatten(Axis::Z);
			result.push_back(rotated);

			childShape.view(Axis::X, true, Axis::Y, true);
		}
		break;
	case Component::RING: {
		Shape face = Shape(childShape);
		face.view(Axis::Y, true, Axis::X, true);
		face.flatten(Axis::Z);
		result.push_back(face);

		face = Shape(childShape);
		face.view(Axis::X, true, Axis::Y, false);
		face.flatten(Axis::Z);
		result.push_back(face);

		face = Shape(childShape);
		face.view(Axis::Y, false, Axis::X, false);
		face.flatten(Axis::Z);
		result.push_back(face);

		face = Shape(childShape);
		face.view(Axis::X, false, Axis::Y, true);
		face.flatten(Axis::Z);
		result.push_back(face);

		break;
	}
	case Component::POLYGON_WALLS:
		if (hasFloorplan()) {
			std::size_t floorPointCount = getFloorPointCount();
			glm::vec3 parentSize = getSize();
			for (std::size_t i = 0; i < floorPointCount; ++i) {
				glm::vec2 first = getFloorPoint(i);
				glm::vec2 second = getFloorPoint((i + 1) % floorPointCount);

				double xDistance = (second.x - first.x) * parentSize.x;
				double yDistance = (second.y - first.y) * parentSize.y;
				double angle = atan2(yDistance, xDistance);
				double distance = sqrt(pow(yDistance, 2) + pow(xDistance, 2));

				Shape section = Shape(childShape);
				section.relativeTranslate(first.x, 0.0f, first.y);
				section.rotate(Axis::Y, glm::degrees(angle), false);
				section.resize(Axis::X, distance, false);
				section.resize(Axis::Y, parentSize.y, false);
				section.flatten(Axis::Z);
				result.push_back(section);
			}
		}
		break;
	};

	return result;
}