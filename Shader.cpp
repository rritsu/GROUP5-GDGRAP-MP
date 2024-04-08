#include "Shader.h"

Shader::Shader() {
    this->vertShader = NULL;
    this->fragShader = NULL;
}

//links the vertex and fragment shader to shader program
GLuint Shader::createShader(std::string fileVert, std::string fileFrag) {
    this->loadVertAndFrag(fileVert, fileFrag);
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, this->vertShader);
    glAttachShader(shaderProg, this->fragShader);

    return shaderProg;
}

//load vertex and frament shader files
void Shader::loadVertAndFrag(std::string fileVert, std::string fileFrag) {
    std::fstream vertSrc(fileVert);
    if (!vertSrc) {
        std::cout << "Error: File NOT found." << std::endl;
    }
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc(fileFrag);
    if (!fragSrc) {
        std::cout << "Error: File NOT found." << std::endl;
    }
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    this->vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertShader, 1, &v, NULL);
    glCompileShader(this->vertShader);

    this->fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->fragShader, 1, &f, NULL);
    glCompileShader(this->fragShader);
}	


void Shader::deleteShader() {
    this->vertShader = NULL;
    this->fragShader = NULL;
}