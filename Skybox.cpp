#include "Skybox.h"

Skybox::Skybox() {}

/*
void Skybox::createSkyboxVertices(float skyboxVertices[]) {
    skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };
}

void Skybox::createSkyboxIndices(unsigned int skyboxIndices[]) {
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };
}
*/

/*

void Skybox::bindBuffers(float skyboxVertices[24], int vertSize, int indicesSize, unsigned int skyboxIndices[36], unsigned int* skyboxVAO, unsigned int* skyboxVBO, unsigned int* skyboxEBO) {
    //-----------SKYBOX BUFFERS------------------
    //CREATE VAO, VBO, AND EBO

 
    glGenVertexArrays(1, skyboxVAO);
    glGenBuffers(1, skyboxVBO);
    glGenBuffers(1, skyboxEBO);

    //ADD SKYBOX DATA TO THE BUFFERS
    //VAO AND VBO
    glBindVertexArray(*skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, (*skyboxVBO));
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

  //  std::cout << "vert size: " << sizeof(skyboxVertices)/sizeof(float) << std::endl;

}

*/


//creates the vertices and indices for the skybox as well as bind their buffers
void Skybox::createSkyboxData(unsigned int* skyboxVAO) {

    float skyboxVertices[24]{
    -1.f, -1.f, 1.f, //0
    1.f, -1.f, 1.f,  //1
    1.f, -1.f, -1.f, //2
    -1.f, -1.f, -1.f,//3
    -1.f, 1.f, 1.f,  //4
    1.f, 1.f, 1.f,   //5
    1.f, 1.f, -1.f,  //6
    -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[36]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };


    //-----------SKYBOX BUFFERS------------------
    //CREATE VAO, VBO, AND EBO

    unsigned int skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    //ADD SKYBOX DATA TO THE BUFFERS
    //VAO AND VBO
    glBindVertexArray(*skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, (skyboxVBO));
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

}


//the source file of the faces of skyboxes are passed and will be loaded here
unsigned int Skybox::initializeCubeMap(std::string facesSkybox[6]) {
    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    stbi_set_flip_vertically_on_load(true);

    return skyboxTex;

}


//draw skybox and binds view and projection properties to be passed to the shader
void Skybox::drawSkybox(GLuint skybox_shaderProg, GLuint* skyboxVAO, glm::mat4 viewMatrix, glm::mat4 projection, unsigned int skyboxTex) {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skybox_shaderProg);

    //CAST CAMERA
    glm::mat4 sky_view = glm::mat4(1.0f);
    sky_view = glm::mat4(glm::mat3(viewMatrix));


    unsigned int skyboxViewLoc = glGetUniformLocation(skybox_shaderProg, "view");
    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

    unsigned int skyboxProjLoc = glGetUniformLocation(skybox_shaderProg, "projection");
    glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //DRAW SKYBOX
    glBindVertexArray(*skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

}