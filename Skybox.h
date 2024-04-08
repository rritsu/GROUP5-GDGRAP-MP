#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tiny_obj_loader.h"
#include "stb_image.h"

class Skybox
{
public:
	Skybox();

public:
	//	void createSkyboxVertices(float skyboxVertices[]);
	//	void createSkyboxIndices(unsigned int skyboxIndices[]);
	//void bindBuffers(float skyboxVertices[24], int vertSize, int indicesSize, unsigned int skyboxIndices[36], unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxEBO);
	void createSkyboxData(unsigned int* skyboxVAO);
	unsigned int initializeCubeMap(std::string facesSkybox[6]);
	void drawSkybox(GLuint skybox_shaderProg, GLuint* skyboxVAO, glm::mat4 viewMatrix, glm::mat4 projection, unsigned int skyboxTex);
};

//skybox_shaderProg, viewMatrix, projection, skybox vao, skybox Tex