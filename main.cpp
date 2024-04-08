/*
GDGRAP1 - MP
Date: 04-08-24
Group 5 Members:
    Austria, Joshua Angelo E.
    Legaspi, Andrea Maxene F.
*/

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

//class header
#include "Model3D.h"
#include "Shader.h"
#include "Skybox.h"
#include "Light/Light.h"
#include "Light/PointLight.h"
#include "Light/DirectionLight.h"
#include "Camera/MyCamera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


float width = 800.0f;
float height = 800.0f;


bool firstMouse = true;
std::string projectionType = "Perspective";
std::string POV = "Third Person";
std::string lastPOV = "Third Person";

//Model3D* model;
Model3D* psub = new Model3D();
Model3D* esub1 = new Model3D();
Model3D* esub2 = new Model3D();
Model3D* esub3 = new Model3D();
Model3D* esub4 = new Model3D();
Model3D* esub5 = new Model3D();
Model3D* esub6 = new Model3D();
Model3D* lightSource = new Model3D();
Model3D* activeModel = new Model3D();

PerspectiveCamera* persCamera = new PerspectiveCamera();
OrthoCamera* orthoCamera = new OrthoCamera();

Light* light = new Light();
PointLight* pointLight = new PointLight();
DirectionLight* directionLight = new DirectionLight();

int i = 1;
double last_xpos = 0.0f;
double last_ypos = 0.0f;


//this function handles the mouse control for the camera
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    //INITIALIZE MOUSE
    persCamera->initialize(&firstMouse, xpos, ypos);

    if (projectionType == "Perspective" && POV == "Third Person") {
        persCamera->updateMouse(xpos, ypos);
    }
    else {
        if (i == 1) {
            last_xpos = xpos;
            last_ypos = ypos;
        }
        i++;
    }

}



void processInput(GLFWwindow* window)
{
    int type = 0;

    //VARIATIONS FOR POV
    if (POV == "Third Person") {
        type = 1;
    }
    else {
        type = 2;
    }

    float pos_x = psub->getPos_X();
    float pos_y = psub->getPos_Y();
    float pos_z = psub->getPos_Z();

    //---------------WASDQE MOVEMENT------------------

    //FORWARD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        psub->setPos_Z(pos_z - 0.1f);
        psub->setTheta_Y(0.0f);
        persCamera->updatePosition(type, pos_x, pos_y, pos_z);
        pointLight->setLightPos(glm::vec3(pos_x, pos_y - 0.02, pos_z + 1.0f));

    }

    //BACKWARD
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        psub->setPos_Z(pos_z + 0.1f);
        psub->setTheta_Y(180.0f);
        persCamera->updatePosition(type, pos_x, pos_y, pos_z);
        pointLight->setLightPos(glm::vec3(pos_x, pos_y - 0.02, pos_z + 1.0f));
    }

    //LEFT
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        psub->setPos_X(pos_x - 0.1f);
        psub->setTheta_Y(270.0f);
        persCamera->updatePosition(type, pos_x, pos_y, pos_z);
        pointLight->setLightPos(glm::vec3(pos_x - 0.5f, pos_y - 0.02, pos_z - 0.1f));
    }

    //RIGHT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        psub->setPos_X(pos_x + 0.1f);
        psub->setTheta_Y(90.0f);
        persCamera->updatePosition(type, pos_x, pos_y, pos_z);
        pointLight->setLightPos(glm::vec3(pos_x + 0.5f, pos_y - 0.02, pos_z - 0.1f));
    }

    //DESCEND
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        psub->setPos_Y(pos_y - 0.1f);
        persCamera->updatePosition(type, pos_x, pos_y, pos_z);
        pointLight->setLightPos(glm::vec3(pos_x, pos_y - 0.02, pos_z - 0.1f));
        std::cout << "Depth: " << pos_y << std::endl;
    }

    //ASCENDD
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

        //WILL ONLY ASCEND IF POSIITON Y IS UNDER OR EQUAL TO 0
        if (pos_y <= 0.0f) {
            psub->setPos_Y(pos_y + 0.1f);
            persCamera->updatePosition(type, pos_x, pos_y, pos_z);
            pointLight->setLightPos(glm::vec3(pos_x, pos_y - 0.02, pos_z - 0.1f));
           
        }
        std::cout << "Depth: " << pos_y << std::endl;
    }

}




void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode) {


    //-------EXIT BUTTON-----------
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    float pos_x = psub->getPos_X();
    float pos_y = psub->getPos_Y();
    float pos_z = psub->getPos_Z();

    //SWITCH TO PERSPECTIVE CAMERA
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {

        //TOGGLE TO THIRD PERSON
        if (POV == "Third Person") {
            std::cout << "Changed to First Person POV" << std::endl;
            POV = "First Person";
            projectionType = "Perspective";
            persCamera->setCameraPos(glm::vec3(pos_x, pos_y, pos_z - 4.0f));
            lastPOV = "First Person";
        }

        //TOGGLE TO FIRST PERSON
        else if (POV == "First Person") {
            POV = "Third Person";
            i = 0;
            std::cout << "Changed to Third Person POV" << std::endl;
            projectionType = "Perspective";
            persCamera->setCameraPos(glm::vec3(pos_x, pos_y + 0.3f, pos_z + 1.15f));
            lastPOV = "Third Person";
        }
    }

    //SWITCH TO ORTHOGRAPHIC CAMERA
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        if (projectionType == "Ortho") {
            std::cout << "Already using ortho projection" << std::endl; 
        }
        else {
            projectionType = "Ortho";
            std::cout << "Changed projection type" << std::endl; 
            
        }
    }

    //INTENSITY MODIFICATION
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        float intensity = pointLight->getIntensity() + 1;

        if (intensity > 3) {    //  <------ implements the cycle of intensity
            intensity = 1;
        }
        pointLight->setIntensity(intensity);

        switch ((int)intensity) {
            case 1: 
                std::cout << "Intensity changed to: LOW" << std::endl; 
                break;

            case 2:
                std::cout << "Intensity changed to: MEDIUM" << std::endl;
                break;

            case 3:
                std::cout << "Intensity changed to: HIGH" << std::endl;
                break;
        }
    }
}


int main(void) {
    GLFWwindow* window;

    //INITIATE LIBRARY
    if (!glfwInit())
        return -1;

    //CREATE WINDOW
    window = glfwCreateWindow(width, height, "Legaspi,Andrea and Austria,Joshua", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //INPUT CALLBACK
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, width, height);


    //STORE MODELS INTO A VECTOR
    std::vector <Model3D*> vecModels;
    vecModels.push_back(psub);
    vecModels.push_back(esub1);
    vecModels.push_back(esub2);
    vecModels.push_back(esub3);
    vecModels.push_back(esub4);
    vecModels.push_back(esub5);
    vecModels.push_back(esub6);



    //--------------SHADER------------------
    Shader* shader = new Shader();
    GLuint shaderProg = shader->createShader("Shaders/model.vert", "Shaders/model.frag");
    glLinkProgram(shaderProg);
    shader->deleteShader();
    


    //--------------PLAYER------------------
    //source: https://www.turbosquid.com/3d-models/3d-titan-submersible-2087088
    //LOAD TEXTURE AND NORMAL MAPS
    GLuint ptexture = psub->loadTexture(1, "3D/player/player_base.png");
    GLuint pNorm_tex = psub->loadNormalMap(1, "3D/player/player_normal.png");

    //VERTEX DATA
    std::vector<GLfloat> pfullVertexData = psub->loadModel("3D/player/player.obj");
  
    //BIND BUFFERS   
    GLuint pVAO, pVBO;
    psub->bindBuffers(pfullVertexData, &pVAO, &pVBO);
    std::cout << "player model loaded" << std::endl;

  

    //--------------ENEMY 1-----------------
    //source: https://www.cgtrader.com/free-3d-models/military/military-vehicle/submarine-0a98eb1b-a71d-45b9-8ed6-66640987efb6
    GLuint e1texture = esub1->loadTexture(1, "3D/enemy1/enemy1_base.jpg");
    GLuint e1Norm_tex = esub1->loadNormalMap(2, "3D/enemy1/enemy1_normal.png");
    std::vector<GLfloat> e1fullVertexData = esub1->loadModel("3D/enemy1/enemy1.obj");

    GLuint e1VAO, e1VBO;
    esub1->bindBuffers(e1fullVertexData, &e1VAO, &e1VBO);
    std::cout << "enemy1 model loaded" << std::endl;


 
    //--------------ENEMY 2-----------------
    //source: https://www.cgtrader.com/3d-model/bathyscaphe-c
    GLuint e2texture = esub2->loadTexture(1, "3D/enemy2/enemy2_base.png");
    GLuint e2Norm_tex = esub2->loadNormalMap(1, "3D/enemy2/enemy2_normal.png");
    std::vector<GLfloat> e2fullVertexData = esub2->loadModel("3D/enemy2/enemy2.obj");

    GLuint e2VAO, e2VBO;
    esub2->bindBuffers(e2fullVertexData, &e2VAO, &e2VBO);
    std::cout << "enemy2 model loaded" << std::endl;



    //--------------ENEMY 3-----------------
    //souce: https://www.cgtrader.com/3d-model/fantastic-boat
    GLuint e3texture = esub3->loadTexture(1, "3D/enemy3/enemy3_base.png");
    GLuint e3Norm_tex = esub3->loadNormalMap(1, "3D/enemy3/enemy3_normal.png");
    std::vector<GLfloat> e3fullVertexData = esub3->loadModel("3D/enemy3/enemy3.obj");

    GLuint e3VAO, e3VBO;
    esub3->bindBuffers(e3fullVertexData, &e3VAO, &e3VBO);
    std::cout << "enemy3 model loaded" << std::endl;



    //--------------ENEMY 4-----------------
    //souce: 
    GLuint e4texture = esub4->loadTexture(1, "3D/enemy4/enemy4_base.png");
    GLuint e4Norm_tex = esub4->loadNormalMap(2, "3D/enemy4/enemy4_normal.png");
    std::vector<GLfloat> e4fullVertexData = esub4->loadModel("3D/enemy4/enemy4.obj");

    GLuint e4VAO, e4VBO;
    esub4->bindBuffers(e4fullVertexData, &e4VAO, &e4VBO);
    std::cout << "enemy4 model loaded" << std::endl;




    //--------------ENEMY 5-----------------
    //source: https://github.com/ThiagoPereiraUFV/Submarine-Simulator/blob/master/models/submarine.obj
    //https://github.com/ThiagoPereiraUFV/Submarine-Simulator/blob/master/models/texture/ship.bmp 
    GLuint e5texture = esub5->loadTexture(1, "3D/enemy5/enemy5_base.png");
    GLuint e5Norm_tex = esub5->loadNormalMap(2, "3D/enemy5/enemy5_normal.png");
    std::vector<GLfloat> e5fullVertexData = esub5->loadModel("3D/enemy5/enemy5.obj");

    GLuint e5VAO, e5VBO;
    esub5->bindBuffers(e5fullVertexData, &e5VAO, &e5VBO);
    std::cout << "enemy5 model loaded" << std::endl;
   
 

    //--------------ENEMY 6-----------------
    //source: https://free3d.com/3d-model/tugboat-v1--395993.html
    GLuint e6texture = esub6->loadTexture(1, "3D/enemy6/enemy6_base.png");
    GLuint e6Norm_tex = esub6->loadNormalMap(1, "3D/enemy6/enemy6_normal.png");
    std::vector<GLfloat> e6fullVertexData = esub6->loadModel("3D/enemy6/enemy6.obj");

    GLuint e6VAO, e6VBO;
    esub6->bindBuffers(e6fullVertexData, &e6VAO, &e6VBO);
    std::cout << "enemy6 model loaded" << std::endl;
    

    //--------------SKYBOX------------------
    Shader* skybox_shader = new Shader();
    GLuint skybox_shaderProg = skybox_shader->createShader("Shaders/sky.vert", "Shaders/sky.frag");
    glLinkProgram(skybox_shaderProg);
    skybox_shader->deleteShader();

    Skybox* skybox = new Skybox();

    unsigned int skyboxVAO;
    skybox->createSkyboxData(&skyboxVAO);


    //----------SKYBOX FACES SOURCE-----------
    std::string facesSkybox[]{
        "Skybox/ocean_rt.png",
        "Skybox/ocean_lf.png",
        "Skybox/ocean_up.png",
        "Skybox/ocean_dn.png",
        "Skybox/ocean_ft.png",
        "Skybox/ocean_bk.png"
    };

    //-----------LOAD SKYBOX FACES AND BINDING-------------
    unsigned int skyboxTex = skybox->initializeCubeMap(facesSkybox);


    //INITIALIZE PLAYER POSITION
    psub->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    psub->setTheta_Y(0.0f);
    psub->setPosition(glm::vec3(0.0f, -20.0f, 1.0f));
    persCamera->setCameraPos(glm::vec3(0.0f, psub->getPos_Y() + 0.3f, psub->getPos_Z() + 1.15f));
    pointLight->setLightPos(glm::vec3(0.0f, psub->getPos_Y() - 0.02, psub->getPos_Z() - 0.1f));


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //INPUT PROCESSING
        processInput(window);
        glfwSetKeyCallback(window, Key_Callback);


        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        //PERSPECTIVE CAMERA
        if (projectionType == "Perspective") {
            //THIRD PERSON POV
            if (POV == "Third Person") {
                projection = persCamera->giveProjection(width, height);
                viewMatrix = persCamera->giveView(1);
            }

            //FIRST PERSON POV
            else {
                projection = persCamera->giveProjection(width, height);
                viewMatrix = persCamera->giveView(1);
            }
        }

        //ORTHOGRAPHIC CAMERA
        else if (projectionType == "Ortho") {
            projection = orthoCamera->giveProjection();
        }


        //--------------DRAW SKYBOX---------------
        skybox->drawSkybox(skybox_shaderProg, &skyboxVAO, viewMatrix, projection, skyboxTex);


        //GET CAMERA POS
        glm::vec3 cameraPos = persCamera->getCameraPos();


        //GENERAL LIGHT PROPERTIES
        glm::vec3 lightColor = light->getLightColor();
        glm::vec3 ambientColor = light->getAmbientColor();


        //DIRECTION LIGHT PROPERTIES
        glm::vec3 lightDirection = directionLight->getLightDirection();
        glm::vec3 dir_lightPos = directionLight->getLightPos();
        float dir_brightness = directionLight->getBrightness();
        float dir_ambientStr = directionLight->getAmbientStr();
        float dir_specStr = directionLight->getSpecStr();
        float dir_specPhong = directionLight->getSpecPhong();

        //POINT LIGHT PROPERTIES
        glm::vec3 point_lightPos = pointLight->getLightPos();
        float point_brightness = pointLight->getBrightness();
        float point_ambientStr = pointLight->getAmbientStr();
        float point_specStr = pointLight->getSpecStr();
        float point_specPhong = pointLight->getSpecPhong();
        float intensity = pointLight->getIntensity();
        

        //-----------------BINDING OF PROPERTIES-----------------

        for (int i = 0; i < vecModels.size(); i++) {
            glUseProgram(shaderProg);

            vecModels[i]->bindCamera(shaderProg, projection, viewMatrix, cameraPos);

            //DIRECTION LIGHT
            vecModels[i]->bindLight(shaderProg, dir_lightPos, lightColor, dir_brightness, "dir_lightPos", "dir_brightness");
            vecModels[i]->bindAmbient(shaderProg, dir_ambientStr, ambientColor, "dir_ambientStr");
            vecModels[i]->bindSpec(shaderProg, dir_specStr, dir_specPhong, "dir_specStr", "dir_specPhong");
            vecModels[i]->bindLightDirection(shaderProg, lightDirection);

            //POINT LIGHT
            vecModels[i]->bindLight(shaderProg, point_lightPos, lightColor, point_brightness, "point_lightPos", "point_brightness");
            vecModels[i]->bindAmbient(shaderProg, point_ambientStr, ambientColor, "point_ambientStr");
            vecModels[i]->bindSpec(shaderProg, point_specStr, point_specPhong, "point_specStr", "point_specPhong");
            vecModels[i]->bindLightIntensity(shaderProg, intensity);
            vecModels[i]->bindLightDirection(shaderProg, lightDirection);
        }


        //TOTAL NUMBER OF INDICES IN FULL VERTEX DATA
        int numVertices = 14;

        //--------------DRAW MODELS---------------
        psub->drawModel(pfullVertexData, ptexture, pNorm_tex, shaderProg, &pVAO, numVertices);

        
        //--------------ENEMY PROPERTIES AND RENDER------------
        esub1->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
        esub1->setPosition(glm::vec3(-15.0f, -10.0f, -30.0f));
        esub1->drawModel(e1fullVertexData, e1texture, e1Norm_tex, shaderProg, &e1VAO, numVertices);
       
        esub2->setScale(glm::vec3(1.5f, 1.5f, 1.5f));
        esub2->setPosition(glm::vec3(14.5f, -0.5f, 30.0f));
        esub2->drawModel(e2fullVertexData, e2texture, e2Norm_tex, shaderProg, &e2VAO, numVertices);
      
        esub3->setScale(glm::vec3(3.0f, 3.0f, 3.0f));
        esub3->setPosition(glm::vec3(-10.2f, -20.0f, -10.0f));
        esub3->drawModel(e3fullVertexData, e3texture, e3Norm_tex, shaderProg, &e3VAO, numVertices);

        esub4->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
        esub4->setPosition(glm::vec3(20.8f, -15.0f, -20.8f));
        esub4->drawModel(e4fullVertexData, e4texture, e4Norm_tex, shaderProg, &e4VAO, numVertices);
         
        esub5->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
        esub5->setPosition(glm::vec3(-5.8f, -10.0f, 20.0f));
        esub5->drawModel(e5fullVertexData, e5texture, e5Norm_tex, shaderProg, &e5VAO, numVertices);

        esub6->setScale(glm::vec3(0.9f, .9f, 0.9f));
        esub6->setPosition(glm::vec3(10.4f, -5.0f, 4.0f));
        esub6->drawModel(e6fullVertexData, e6texture, e6Norm_tex, shaderProg, &e6VAO, numVertices);

        

        //swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    //--------------DELETE BUFFERS---------------
    glDeleteVertexArrays(1, &pVAO);
    glDeleteBuffers(1, &pVBO);
    
   
    glDeleteVertexArrays(1, &e1VAO);
    glDeleteBuffers(1, &e1VBO);

    glDeleteVertexArrays(1, &e2VAO);
    glDeleteBuffers(1, &e2VBO);

    glDeleteVertexArrays(1, &e3VAO);
    glDeleteBuffers(1, &e3VBO);

    glDeleteVertexArrays(1, &e4VAO);
    glDeleteBuffers(1, &e4VBO);
 

    glDeleteVertexArrays(1, &e5VAO);
    glDeleteBuffers(1, &e5VBO);

    glDeleteVertexArrays(1, &e6VAO);
    glDeleteBuffers(1, &e6VBO);

    
    glfwTerminate();

    return 0;
}