#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
	private:
		GLuint vertShader;
		GLuint fragShader;

	public:
		Shader();

	public:
		GLuint createShader(std::string fileVert, std::string fileFrag);
		void loadVertAndFrag(std::string fileVert, std::string fileFrag);
		void deleteShader();

};

