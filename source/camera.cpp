#include "camera.hpp"
#include <vector>
#include <iostream>

// default constructor
camera::camera() {
	projectionMatrix = glm::perspective(45.0f, 3.0f / 4.0f, 0.1f, 100.0f);
	cameraPosition = glm::vec3(0, 0, 15);
	cameraUp = glm::vec3(0, 1, 0);
	cameraRight = glm::vec3(1, 0, 0);
	cameraSpeed = 4.0f;
}
camera::camera(bool isOrthographic, float cameraSpeed) {
	// orthographic camera vs perspective camera
	if (isOrthographic) {
		projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates
	}
	else {
		projectionMatrix = glm::perspective(45.0f, 3.0f / 4.0f, 0.1f, 100.0f);
	}
	this->cameraSpeed = cameraSpeed;
}

void camera::setCameraSpeed(float newSpeed) {
	cameraSpeed = newSpeed;
}

void camera::makeOrthographic(bool isOrthographic) {
	// orthographic camera vs perspective camera
	if (isOrthographic) {
		projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates
	}
	else {
		projectionMatrix = glm::perspective(45.0f, 3.0f / 4.0f, 0.1f, 100.0f);
	}
}

void camera::moveCamera(int key, float deltaTime) {
    // multiply by delta time to be independent of frame rate
	switch (key) {
	case GLFW_KEY_UP:
		cameraPosition += cameraUp * deltaTime * cameraSpeed; // move camera up along up vector
		break;
	case GLFW_KEY_DOWN:
		cameraPosition += -cameraUp * deltaTime * cameraSpeed; // move camera down along negative up vector
		break;
	case GLFW_KEY_LEFT:
		cameraPosition += -cameraRight * deltaTime * cameraSpeed; // move camera left along negative right vector
		break;
	case GLFW_KEY_RIGHT:
		cameraPosition += cameraRight * deltaTime * cameraSpeed; // move camera right along right vector
		break;
	}

	// update view matrix after moving camera
	setViewMatrix(); 
}

void camera::setViewMatrix() {
	// calculate new right vector
	// take cross product of the vector from cameraPosition to origin and the +y-axis
	cameraRight = glm::cross(glm::vec3(0.0f) - cameraPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	cameraRight = glm::normalize(cameraRight);     // normalize

	// calculate new up vector
	// take cross product of right vector and vector from cameraPosition to origin
	cameraUp = glm::cross(cameraRight, glm::vec3(0.0f) - cameraPosition);
	cameraUp = glm::normalize(cameraUp);    // normalize

	// View Matrix (V): world -> camera space
	// arg1: camera position; arg2: target position; arg3: up vector
	viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0.0f), cameraUp); 
}

glm::mat4 camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 camera::getViewMatrix() {
	return viewMatrix;
}
