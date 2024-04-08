#pragma once

#include "MyCamera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PerspectiveCamera : public MyCamera
{
	public:
		PerspectiveCamera();

	public:
		glm::mat4 giveProjection(float width, float height);
		glm::mat4 giveView(int type);

};

