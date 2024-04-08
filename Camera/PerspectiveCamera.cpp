#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera() {
}


//gives the viewMatrix depending on the projection type
glm::mat4 PerspectiveCamera::giveView(int type) {
    glm::mat4 viewMatrix;

    if(type == 1)
        viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
    else 
        viewMatrix = glm::lookAt(this->cameraPos, this->cameraCenter, this->cameraUp);

    return viewMatrix;
}

//gives the projection of perspective
glm::mat4 PerspectiveCamera::giveProjection(float width, float height) {
    return glm::perspective(glm::radians(this->fov), height / width, this->z_near, this->z_far);
}