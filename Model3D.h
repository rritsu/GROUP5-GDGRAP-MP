#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "tiny_obj_loader.h"

class Model3D
{
	//attributes/fields
	private:
		float pos_x;
		float pos_y;
		float pos_z;
		float scale_x;
		float scale_y;
		float scale_z;
		float axis_x;
		float axis_y;
		float axis_z;
		float theta_x;
		float theta_y;
		float theta_z;
	//	float theta;
		glm::vec4 modelColor;

	//constructor
	public:
		Model3D();

	//methods/behavior
	public:
		GLuint loadTexture(int type, std::string imgSrc);
		GLuint loadNormalMap(int type, std::string imgSrc);
		std::vector<GLfloat> loadModel(std::string objSrc);
		void bindBuffers(std::vector<GLfloat> fullVertexData, GLuint* VAO, GLuint* VBO);
		void drawModel(std::vector<GLfloat> fullVertexData, GLuint texture, GLuint norm_tex, GLuint shaderProg, GLuint* VAO, int numVertices);
		void drawLightPoint(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* VAO);
		void setPosition(glm::vec3 newPos);
		glm::vec3 getPosition() const;
		void setScale(glm::vec3 scale);
		glm::vec3 getScale() const;
		

		void bindCamera(GLuint shaderProg, glm::mat4 projection, glm::mat4 viewMatrix, glm::vec3 cameraPos);
		void bindLight(GLuint shaderProg, glm::vec3 lightPos, glm::vec3 lightColor, float brightness, std::string strLightPos, std::string strBrightness);
		void bindAmbient(GLuint shaderProg, float ambientStr, glm::vec3 ambientColor, std::string strAmbientStr);
		void bindSpec(GLuint shaderProg, float specStr, float specPhong, std::string strSpecStr, std::string strSpecPhong);
		void bindLightDirection(GLuint shaderProg, glm::vec3 lightDirection);
		void bindLightIntensity(GLuint shaderProg, float intensity);


	//getters
	public:
		float getPos_X();
		float getPos_Y();
		float getPos_Z();
		void setPos_X(float pos_x);
		void setPos_Y(float pos_y);
		void setPos_Z(float pos_z);

		float getScale_X();
		float getScale_Y();
		float getScale_Z();
		void setScale_X(float scale_x);
		void setScale_Y(float scale_y);
		void setScale_Z(float scale_z);

		float getAxis_X();
		float getAxis_Y();
		float getAxis_Z();
		void setAxis_X(float axis_x);
		void setAxis_Y(float axis_y);
		void setAxis_Z(float axis_z);

		float getTheta_X();
		float getTheta_Y();
		float getTheta_Z();
		void setTheta_X(float theta_x);
		void setTheta_Y(float theta_y);
		void setTheta_Z(float theta_z);
};

