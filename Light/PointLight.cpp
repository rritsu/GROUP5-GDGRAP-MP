#include "PointLight.h"

PointLight::PointLight() {
    this->intensity = 1.0f;
}

float PointLight::getIntensity() {
    return this->intensity;
}

void PointLight::setIntensity(float intensity) {
    this->intensity = intensity;
}