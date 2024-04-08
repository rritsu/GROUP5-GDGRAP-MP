#pragma once

#include "iostream"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCamera
{
	private:
		float cam_x;
		float cam_y;
		float cam_z;
		float yaw;
		float pitch;
		float last_x;
		float last_y;

	protected:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::vec3 cameraCenter;
		float fov;
		float z_far;
		float z_near;

	public:
		MyCamera();

	public:
		void initialize(bool* firstMouse, double xpos, double ypos);
		void updateMouse(double xpos, double ypos);
		glm::mat4 bindView(GLuint shaderProg);
		void bindProjection(GLuint shaderProg, glm::mat4 projection);
		void updatePosition(int type, float x, float y, float z);

	public:
		float getCam_X();
		void setCam_X(float cam_x);
		float getCam_Y();
		void setCam_Y(float cam_y);
		float getCam_Z();
		void setCam_Z(float cam_z);

		glm::vec3 getCameraPos();
		void setCameraPos(glm::vec3 cameraPos);
		glm::vec3 getCameraFront();
		void setCameraFront(glm::vec3 cameraFront);
		glm::vec3 getCameraUp();
		glm::vec3 getCameraCenter();
		void setCameraCenter(glm::vec3 cameraCenter);

		float getZ_Far();
		void setZ_Far(float z_far);
		float getZ_Near();
		void setZ_Near(float z_near);
};