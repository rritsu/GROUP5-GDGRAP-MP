#include "OrthoCamera.h"

OrthoCamera::OrthoCamera() {
	this->leftMost = -10.0f;
	this->rightMost = 10.0f;
	this->topMost = 10.0f;
	this->bottomMost = -10.0f;
	this->z_near = 0.1f;
	this->z_far = 1000;
}


glm::mat4 OrthoCamera::giveView() {
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	return viewMatrix;
}

glm::mat4 OrthoCamera::giveProjection() {
	this->z_near = -100.0f;
	this->z_far = 1.0f;
	return glm::ortho(this->leftMost, this->rightMost, this->bottomMost, this->topMost, this->z_near, this->z_far);
}

float OrthoCamera::getLeftMost() {
	return this->leftMost;
}

void OrthoCamera::setLeftMost(float leftMost) {
	this->leftMost = leftMost;
}

float OrthoCamera::getRightMost() {
	return this->rightMost;
}

void OrthoCamera::setRightMost(float rightMos) {
	this->rightMost = rightMos;
}

float OrthoCamera::getBottomMost() {
	return this->bottomMost;
}

void OrthoCamera::setBottomMost(float bottomMost) {
	this->bottomMost = bottomMost;
}

float OrthoCamera::getTopMost() {
	return this->topMost;
}

void OrthoCamera::setTopMost(float topmost) {
	this->topMost = topMost;
}

