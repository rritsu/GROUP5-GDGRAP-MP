#include "Light.h"

Light::Light() {
	this->lightPos = glm::vec3(0.0f, 0.0f, 1.0f);
	this->lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	this->ambientColor = this->lightColor;
	this->ambientStr = 0.2f;
	this->specStr = 0.2f;
	this->specPhong = 5.0f;
	this->brightness = 1.0f;
}


//getters and setters
glm::vec3 Light::getLightPos() {
	return this->lightPos;
}

void Light::setLightPos(glm::vec3 lightPos) {
	this->lightPos = lightPos;
}


glm::vec3 Light::getLightColor() {
	return this->lightColor;
}

void Light::setLightColor(glm::vec3 lightColor) {
	this->lightColor = lightColor;
}

glm::vec3 Light::getAmbientColor() {
	return this->ambientColor;
}

void Light::setAmbientColor(glm::vec3 ambientColor) {
	this->ambientColor = ambientColor;
}

float Light::getAmbientStr() {
	return this->ambientStr;
}

void Light::setAmbientStr(float ambientStr) {
	this->ambientStr = ambientStr;
}

float Light::getSpecStr() {
	return this->specStr = specStr;
}

void Light::setSpecStr(float specStr) {
	this->specStr = specStr;
}

float Light::getSpecPhong() {
	return this->specPhong;
}

void Light::setSpecPhong(float specPhong) {
	this->specPhong = specPhong;
}

float Light::getBrightness() {
	return this->brightness;
}

void Light::setBrightness(float brightness) {
	this->brightness = brightness;
}