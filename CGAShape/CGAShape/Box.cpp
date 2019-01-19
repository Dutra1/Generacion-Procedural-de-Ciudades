#include "stdafx.h"
#include "Box.h"

Box::Box() {
	matrix = glm::mat4();
	size = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Box::translate(float x, float y, float z) {
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
}

void Box::translate(const glm::vec3 & translation) {
	translate(translation.x, translation.y, translation.z);
}

void Box::translate(Axis axis, float value) {
	switch(axis) {
	case Axis::X:
		translate(value, 0.0f, 0.0f);
		break;
	case Axis::Y:
		translate(0.0f, value, 0.0f);
		break;
	case Axis::Z:
		translate(0.0f, 0.0f, value);
		break;
	}
}

void Box::relativeTranslate(float x, float y, float z) {
	translate(x * size.x, y * size.y, z * size.z);
}

void Box::relativeTranslate(Axis axis, float value) {
	translate(axis, value * getSize(axis));
}

void Box::rotate(Axis axis, float angle, bool center) {
	if (center) {
		relativeTranslate(0.5f, 0.5f, 0.5f);
		rotate(axis, angle, false);
		relativeTranslate(-0.5f, -0.5f, -0.5f);
	} else {
		angle = glm::radians(angle);
		switch (axis) {
		case Axis::X:
			matrix = glm::rotate(matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case Axis::Y:
			matrix = glm::rotate(matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case Axis::Z:
			matrix = glm::rotate(matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		}
	}
	
}

void Box::resize(float x, float y, float z, bool center) {
	if (center) {
		translate((size.x - x) / 2.0f, (size.y - y) / 2.0f, (size.z - z) / 2.0f);
		resize(x, y, z, false);
	} else {
		size = glm::vec3(x, y, z);
	}
}

void Box::resize(const glm::vec3 & size, bool center) {
	resize(size.x, size.y, size.z, center);
}

void Box::resize(Axis axis, float value, bool center) {
	if (center) {
		translate(axis, (getSize(axis) - value) / 2.0f);
		resize(axis, value, false);
	} else {
		switch (axis) {
		case Axis::X:
			size.x = value;
			break;
		case Axis::Y:
			size.y = value;
			break;
		case Axis::Z:
			size.z = value;
			break;
		}
	}
}

void Box::scale(float x, float y, float z, bool center) {
	resize(size.x * x, size.y * y, size.z * z, center);
}

void Box::scale(Axis axis, float value, bool center) {
	resize(axis, getSize(axis) * value, center);
}

void Box::scale(float value, bool center) {
	resize(size.x * value, size.y * value, size.z * value, center);
}

void Box::mirror(Axis axis) {
	relativeTranslate(axis, 1.0f);
	scale(axis, -1.0f);
}

void Box::flatten(Axis axis) {
	resize(axis, 0);
}

void Box::view(Component component) {
	switch (component) {
	case Component::BACK:
		view(Axis::Z, true);
		break;
	case Component::LEFT:
		view(Axis::X, false);
		break;
	case Component::RIGHT:
		view(Axis::X, true);
		break;
	case Component::TOP:
		view(Axis::Y, true);
		break;
	case Component::BOTTOM:
		view(Axis::Y, false);
		break;
	};
}

void Box::view(Axis face, bool faceDirection, Axis up, bool upDirection) {
	if (face != up) {
		view(face, faceDirection);
		if (face == Axis::X && faceDirection) {
			if (up == Axis::Z && upDirection) rotateView(Axis::X, true);
			else if (up == Axis::Z && !upDirection) rotateView(Axis::X, false);
			else if (up == Axis::Y && !upDirection) rotateView(Axis::Y, false);
		} else if (face == Axis::X && !faceDirection) {
			if (up == Axis::Z && upDirection) rotateView(Axis::X, false);
			else if (up == Axis::Z && !upDirection) rotateView(Axis::X, true);
			else if (up == Axis::Y && !upDirection) rotateView(Axis::Y, false);
		} else if (face == Axis::Y && faceDirection) {
			if (up == Axis::X && upDirection) rotateView(Axis::X, true);
			else if (up == Axis::X && !upDirection) rotateView(Axis::X, false);
			else if (up == Axis::Z && !upDirection) rotateView(Axis::Y, false);
		} else if (face == Axis::Y && !faceDirection) {
			if (up == Axis::X && upDirection) rotateView(Axis::X, true);
			else if (up == Axis::X && !upDirection) rotateView(Axis::X, false);
			else if (up == Axis::Z && upDirection) rotateView(Axis::Y, false);
		} else if (face == Axis::Z && faceDirection) {
			if (up == Axis::X && upDirection) rotateView(Axis::X, false);
			else if (up == Axis::X && !upDirection) rotateView(Axis::X, true);
			else if (up == Axis::Y && !upDirection) rotateView(Axis::Y, false);
		} else if (face == Axis::Z && !faceDirection) {
			if (up == Axis::X && upDirection) rotateView(Axis::X, true);
			else if (up == Axis::X && !upDirection) rotateView(Axis::X, false);
			else if (up == Axis::Y && !upDirection) rotateView(Axis::Y, false);
		}
	}
}

void Box::view(Axis axis, bool from) {
	switch (axis) {
	case Axis::X:
		if (from) {
			relativeTranslate(Axis::X, 1.0f);
			rotate(Axis::Y, -90.0f, false);
			std::swap(size.x, size.z);
		} else {
			relativeTranslate(Axis::Z, 1.0f);
			rotate(Axis::Y, 90.0f, false);
			std::swap(size.x, size.z);
		}
		break;
	case Axis::Y:
		if (from) {
			relativeTranslate(Axis::Y, 1.0f);
			rotate(Axis::X, 90.0f, false);
			std::swap(size.z, size.y);
		} else {
			relativeTranslate(Axis::Z, 1.0f);
			rotate(Axis::X, -90.0f, false);
			std::swap(size.z, size.y);
		}
		break;
	case Axis::Z:
		if (from) {
			relativeTranslate(1.0f, 0.0f, 1.0f);
			rotate(Axis::Y, 180.0f, false);
		}
		break;
	}
}

void Box::rotateView(Axis axis, bool from) {
	switch (axis) {
	case Axis::X:
		if (from) {
			relativeTranslate(Axis::Y, 1.0f);
			rotate(Axis::Z, -90.0f, false);
			std::swap(size.x, size.y);
		} else {
			relativeTranslate(Axis::X, 1.0f);
			rotate(Axis::Z, 90.0f, false);
			std::swap(size.x, size.y);
		}
		break;
	case Axis::Y:
		if (!from) {
			relativeTranslate(1.0f, 1.0f, 0.0f);
			rotate(Axis::Z, 180.0f, false);
		}
	}
}

glm::mat4 Box::getMatrix() const {
	return matrix;
}

glm::vec3 Box::getPosition() const {
	return glm::vec3(matrix[3]);
}

glm::vec3 Box::getSize() const {
	return size;
}

float Box::getPosition(Axis axis) const {
	switch (axis) {
	case Axis::X: return getPosition().x;
	case Axis::Y: return getPosition().y;
	case Axis::Z: return getPosition().z;
	default: return 0;
	}
}

float Box::getSize(Axis axis) const {
	switch (axis) {
	case Axis::X: return size.x;
	case Axis::Y: return size.y;
	case Axis::Z: return size.z;
	default: return 0;
	}
}