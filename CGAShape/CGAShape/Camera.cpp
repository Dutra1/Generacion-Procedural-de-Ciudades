#include "stdafx.h"
#include "Camera.h"
#include "Config.h"

Camera::Camera() {
	position = Config::cameraPosition;
	direction = glm::normalize(-position);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	fov = 45.0f;
	aspectRatio = Config::screenWidth / (float) Config::screenHeight;
	near = 0.1f;
	far = 10000.0f;

	speed = Config::cameraSpeed;
	rotationSpeed = 0.1f;
	maxVerticalRotation = 0.99f;

	recalculateView();
	recalculateProjection();
}

void Camera::rotate(float horizontalAngle, float verticalAngle) {
	direction = glm::rotate(direction, glm::radians(horizontalAngle * rotationSpeed), up);
	glm::vec3 newDirection = glm::rotate(direction, glm::radians(-verticalAngle * rotationSpeed), glm::cross(direction,up));
	float dot = glm::dot(newDirection, up);
	if (dot < maxVerticalRotation && dot > -maxVerticalRotation) {
		direction = newDirection;
	}
	recalculateView();
}

void Camera::move(float angle, float amount) {
	position += glm::rotate(glm::vec3(direction.x, 0.0f, direction.z), glm::radians(angle), up) * speed * amount;
	recalculateView();
}

void Camera::climb(float amount) {
	position.y += speed * amount;
	if (position.y < 1) {
		position.y = 1;
	}
	recalculateView();
}

void Camera::fall(float amount) {
	this->climb(-amount);
}

void Camera::recalculateView() {
	view = glm::lookAt(position, position + direction, up);
}

void Camera::recalculateProjection() {
	projection = glm::perspective(glm::radians(fov), -aspectRatio, near, far);
}

glm::mat4 Camera::getView() {
	return view;
}

glm::mat4 Camera::getProjection() {
	return projection;
}

glm::vec3 Camera::getPosition() {
	return position;
}