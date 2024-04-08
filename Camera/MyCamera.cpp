#include "MyCamera.h"

MyCamera::MyCamera() {
    this->cam_x = 0.0f;
    this->cam_y = 0.0f;
    this->cam_z = 1.0f;
    this->cameraPos = glm::vec3(this->cam_x, this->cam_y, this->cam_z);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    //mouse position/stateS
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->fov = 45.0f;
    this->last_x = 800.0f / 2.0;
    this->last_y = 600.0 / 2.0;

	//this->projection = glm::mat4(0.0f);
	this->z_far = 1.0f;
	this->z_near = -1.0f;
}

void MyCamera::initialize(bool* firstMouse, double xpos, double ypos) {
    if (*firstMouse)
    {
        //MyCamera camera;
        this->last_x = xpos;
        this->last_y = ypos;
        *firstMouse = false;
    }
}

void MyCamera::updateMouse(double xpos, double ypos) {
    float xoffset = xpos - this->last_x;
    float yoffset = this->last_y - ypos;
    this->last_x = xpos;
    this->last_y = ypos;

    //this logic shows if the higher the sensitivity the larger the mouse movements
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    //capped to 90 in order to avoid flipping the perspective
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    //camera vector to update camera front 
    glm::vec3 front = glm::vec3(1.0f);
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->cameraFront = glm::normalize(front);
}


glm::mat4 MyCamera::bindView(GLuint shaderProg) {
    //camera
    glUseProgram(shaderProg);
    glm::mat4 viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
    unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    return viewMatrix;
}

void MyCamera::bindProjection(GLuint shaderProg, glm::mat4 projection) {
    glUseProgram(shaderProg);
  //  this->setProjection(projection);
    unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void MyCamera::updatePosition(int type, float x, float y, float z) {
    glm::vec3 newPos;
    if (type == 1) {  //third person
        newPos = glm::vec3(x, y + 0.3f, z + 1.15f);
    }
    else {
        newPos = glm::vec3(x, y, z - 4.0f);
    }

    this->cameraPos = newPos;
}


float MyCamera::getCam_X() {
    return this->cam_x;
}

void MyCamera::setCam_X(float cam_x) {
    this->cam_x = cam_x;
}

float MyCamera::getCam_Y() {
    return this->cam_y;
}

void MyCamera::setCam_Y(float cam_y) {
    this->cam_y = cam_y;
}

float MyCamera::getCam_Z() {
    return this->cam_z;
}

void MyCamera::setCam_Z(float cam_z) {
    this->cam_z = cam_z;
}


glm::vec3 MyCamera::getCameraPos() {
    return this->cameraPos;
}

void MyCamera::setCameraPos(glm::vec3 cameraPos) {
    this->cameraPos = cameraPos;
}

glm::vec3 MyCamera::getCameraFront() {
    return this->cameraFront;
}

void MyCamera::setCameraFront(glm::vec3 cameraFront) {
    this->cameraFront = cameraFront;
}

glm::vec3 MyCamera::getCameraUp() {
    return this->cameraUp;
}

glm::vec3 MyCamera::getCameraCenter() {
    return this->cameraCenter;
}

void MyCamera::setCameraCenter(glm::vec3 cameraCenter) {
    this->cameraCenter = cameraCenter;
}

float MyCamera::getZ_Far() {
	return this->z_far;
}

void MyCamera::setZ_Far(float z_far) {
	this->z_far = z_far;
}

float MyCamera::getZ_Near() {
	return this->z_near;
}

void MyCamera::setZ_Near(float z_near) {
	this->z_near = z_near;
}