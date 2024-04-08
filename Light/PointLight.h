#pragma once

#include "Light.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

    class PointLight : public Light{

    private:
        float intensity;


    public:
        PointLight();

    public:
        float getIntensity();
        void setIntensity(float intensity);

};