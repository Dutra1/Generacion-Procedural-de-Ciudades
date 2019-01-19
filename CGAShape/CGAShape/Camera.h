#pragma once
#ifndef CAMERA
#define CAMERA

class Camera {
private:
	glm::vec3 position, direction, up;
	float fov, aspectRatio, near, far;

	glm::mat4 view, projection;

	float speed, rotationSpeed, maxVerticalRotation;

	void recalculateView();
	void recalculateProjection();
public:
	Camera();
	void rotate(float horizontalAngle, float verticalAngle);
	void move(float angle, float amount = 1.0f);
	void climb(float amount = 1.0f);
	void fall(float amount = 1.0f);

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::vec3 getPosition();
};

#endif