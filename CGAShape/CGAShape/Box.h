#pragma once
#ifndef BOX
#define BOX
#include "glm/glm.hpp"
#include "Axis.h"
#include "Component.h"

class Box {
protected:
	glm::mat4 matrix;
	glm::vec3 size;
public:
	Box();
	
	void translate(float x, float y, float z);
	void translate(const glm::vec3 & translation);
	void translate(Axis axis, float value);
	void relativeTranslate(float x, float y, float z);
	void relativeTranslate(Axis axis, float value);

	void rotate(Axis axis, float angle, bool center = true);
	
	void resize(float x, float y, float z, bool center = false);
	void resize(const glm::vec3 & size, bool center = false);
	void resize(Axis axis, float value, bool center = false);
	void scale(float x, float y, float z, bool center = false);
	void scale(Axis axis, float value, bool center = false);
	void scale(float value, bool center = false);
	
	void mirror(Axis axis);
	void flatten(Axis axis);

	void view(Component component);
	void view(Axis face, bool faceDirection, Axis up, bool upDirection);
	void view(Axis axis, bool from);
	void rotateView(Axis axis, bool from);

	glm::mat4 getMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getSize() const;
	float getPosition(Axis axis) const;
	float getSize(Axis axis) const;
};

#endif
