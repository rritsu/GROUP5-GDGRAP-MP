#include "DirectionLight.h"

DirectionLight::DirectionLight() {
	this->lightDirection = glm::vec3(-0.2f, -0.9f, -0.2f);
}

//binds the light direction variable to be passed to the shader
void DirectionLight::bindLightDirection(GLuint shaderProg) {
	glUseProgram(shaderProg);
	GLuint lightDirectionAddress = glGetUniformLocation(shaderProg, "lightDirection");
	glUniform3fv(lightDirectionAddress, 1, glm::value_ptr(this->lightDirection));
}

void DirectionLight::setUpProperties() {
	this->lightPos = glm::vec3(0.0f, 100.0f, 1.0f);
	this->ambientStr = 2.0f;
	this->specStr = 0.10f;
	this->specPhong = 5.0f;
	this->brightness = 2.0f;
}

//getters and setters
glm::vec3 DirectionLight::getLightDirection() {
	return this->lightDirection;
}

void DirectionLight::setLightDirection(glm::vec3 lightDir) {
	this->lightDirection = lightDir;
}

void DirectionLight::setLightDir_X(float dir_x) {
	this->lightDirection.x = dir_x;
}

void DirectionLight::setLightDir_Y(float dir_y) {
	this->lightDirection.y = dir_y;
}

void DirectionLight::setLightDir_Z(float dir_z) {
	this->lightDirection.z = dir_z;
}