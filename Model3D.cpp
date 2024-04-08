#include "Model3D.h"

Model3D::Model3D() {
    this->pos_x = 0.0f;
    this->pos_y = 0.0f;
    this->pos_z = 0.0f;
    this->scale_x = 0.03f;
    this->scale_y = 0.03f;
    this->scale_z = 0.03f;
    this->axis_x = 0.0f;
    this->axis_y = 1.0f;
    this->axis_z = 0.0f;
    this->theta_x= 0.0f;
    this->theta_y= 0.0f;
    this->theta_z= 0.0f;
    this->modelColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

//loads the texture of a model based on imgSrc
GLuint Model3D::loadTexture(int type, std::string imgSrc) {
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load(imgSrc.c_str(), &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //type 1 = if no alpha channel
    //type 2 = with alpha channel


    if (type == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    }
    else if (type == 2){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

    return texture;
}

//loads the texture of a model based on imgSrc
GLuint Model3D::loadNormalMap(int type, std::string imgSrc) {
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes = stbi_load(imgSrc.c_str(), &img_width, &img_height, &colorChannels, 0);

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if (type == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_bytes);
    }
    else if (type == 2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, normal_bytes);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);

    return norm_tex;
}


// loads the 3d models by pushing the data to a vertex and returning it to main
std::vector<GLfloat> Model3D::loadModel(std::string objSrc) {
    std::string path = objSrc;
    std::vector <tinyobj::shape_t> shapes;
    std::vector <tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[1 + 2];

        //push the position of vertex1
        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );

        //push the position of vertex2
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );

        //push the position of vertex3
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        //UV of vertex1
        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );

        //UV of vertex2
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );

        //UV of vertex3
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        //edges of the trinagle : position delta
        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        //UV delta
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        //TANGENT (T)
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

        //BITANGENT (B)
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        //push the tangent and bitangent
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);

    }


    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        //vertices
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);            // 0
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3 + 1)]);        // 1
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3 + 2)]);        // 2

        //normals
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);             // 3
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3 + 1)]);         // 4
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3 + 2)]);         // 5

        //UVs
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);         // 6
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2 + 1)]);     // 7

        fullVertexData.push_back(tangents[i].x);                                            // 8
        fullVertexData.push_back(tangents[i].y);                                            // 9
        fullVertexData.push_back(tangents[i].z);                                            // 10

        fullVertexData.push_back(bitangents[i].x);                                          // 11
        fullVertexData.push_back(bitangents[i].y);                                          // 12
        fullVertexData.push_back(bitangents[i].z);                                          // 13

    }

    return fullVertexData;
}


//bind buffers by setting up pointers depending on the data type
void Model3D::bindBuffers(std::vector<GLfloat> fullVertexData, GLuint* VAO, GLuint* VBO) {
    GLuint EBO;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(*VAO);  
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    //vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normals
    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normalPtr);
    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(2);

    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangentPtr);
    glEnableVertexAttribArray(3);

    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangentPtr);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


//this behavior is for applying transformation and drawing the model
void Model3D::drawModel(std::vector<GLfloat> fullVertexData, GLuint texture, GLuint norm_tex, GLuint shaderProg, GLuint* VAO, int numVertices) {
    glm::mat4 transformation_matrix;

    //translate
    transformation_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(this->pos_x, this->pos_y, this->pos_z)
    );

    //scale
    transformation_matrix = glm::scale(transformation_matrix,
        glm::vec3(this->scale_x, this->scale_y, this->scale_z)
    );

    //rotate
    //X-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_x),
        glm::normalize(glm::vec3(1, 0, 0))
    );

    //Y-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_y),
        glm::normalize(glm::vec3(0, 1, 0))
    );

    //Z-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_z),
        glm::normalize(glm::vec3(0, 0, 1))
    );

  //  std::cout << "loob: " << this->getAxis_Y() << std::endl;
   
    glUseProgram(shaderProg);

    //bind transformation matrix
    unsigned int transformloc = glGetUniformLocation(shaderProg, "transform");
    glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


    //bind texture
    glActiveTexture(GL_TEXTURE0);
    GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(tex0Address, 0);

    //bind normal 
    glActiveTexture(GL_TEXTURE1);
    GLuint norm_TexAddress = glGetUniformLocation(shaderProg, "norm_tex");
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glUniform1i(norm_TexAddress, 1);

    //draw
    glBindVertexArray(*VAO);
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / numVertices);
}

void Model3D::drawLightPoint(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* sphereVAO) {
    glm::mat4 transformation_matrix;

    //translate
    transformation_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(this->pos_x, this->pos_y, this->pos_x)
    );

    //scale
    transformation_matrix = glm::scale(transformation_matrix,
        glm::vec3(this->scale_x, this->scale_y, this->scale_z)
    );

    //rotate
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_x),
        glm::normalize(glm::vec3(2, 0, 0))
    );

    //Y-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_y),
        glm::normalize(glm::vec3(0, 2, 0))
    );

    //Z-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_z),
        glm::normalize(glm::vec3(0, 0, 2))
    );

    glUseProgram(shaderProg);

    //transform
    unsigned int transformloc = glGetUniformLocation(shaderProg, "transform");
    glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    //draw
    glBindVertexArray(*sphereVAO);
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}


//bind camera properties to be passed to the shader
void Model3D::bindCamera(GLuint shaderProg, glm::mat4 projection, glm::mat4 viewMatrix, glm::vec3 cameraPos) {
    unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
}


//bind light properties to be passed to the shader
void Model3D::bindLight(GLuint shaderProg, glm::vec3 lightPos, glm::vec3 lightColor, float brightness, std::string strLightPos, std::string strBrightness) {
    GLuint lightAddress = glGetUniformLocation(shaderProg, strLightPos.c_str());
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

    GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint brightnessAddress = glGetUniformLocation(shaderProg, strBrightness.c_str());
    glUniform1f(brightnessAddress, brightness);

}


//bind light ambient properties to be passed to the shader
void Model3D::bindAmbient(GLuint shaderProg, float ambientStr, glm::vec3 ambientColor, std::string strAmbientStr) {
    GLuint ambientStrAddress = glGetUniformLocation(shaderProg, strAmbientStr.c_str());
    glUniform1f(ambientStrAddress, ambientStr);

    GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
}


//binds light spec to be passed to the shader
void Model3D::bindSpec(GLuint shaderProg, float specStr, float specPhong, std::string strSpecStr, std::string strSpecPhong) {
    GLuint specStrAddress = glGetUniformLocation(shaderProg, strSpecStr.c_str());
    glUniform1f(specStrAddress, specStr);

    GLuint specPhongAddress = glGetUniformLocation(shaderProg, strSpecPhong.c_str());
    glUniform1f(specPhongAddress, specPhong);
    
}

//binds light direction to be passed to the shader
void Model3D::bindLightDirection(GLuint shaderProg, glm::vec3 lightDirection) {
    GLuint lightDirectionAddress = glGetUniformLocation(shaderProg, "lightDirection");
    glUniform3fv(lightDirectionAddress, 1, glm::value_ptr(lightDirection));
}

//binds light intensity to be passed to the shader
void Model3D::bindLightIntensity(GLuint shaderProg, float intensity) {
    GLuint lightIntensityAddress = glGetUniformLocation(shaderProg, "intensity");
    glUniform1f(lightIntensityAddress, intensity);
}


void Model3D::setPosition(glm::vec3 newPos) {
    pos_x = newPos.x;
    pos_y = newPos.y;
    pos_z = newPos.z;
}

glm::vec3 Model3D::getPosition() const {
    return glm::vec3(pos_x, pos_y, pos_z);
}

void Model3D::setScale(glm::vec3 scale) {
    scale_x = scale.x;
    scale_y = scale.y;
    scale_z = scale.z;
}

glm::vec3 Model3D::getScale() const {
    return glm::vec3(scale_x, scale_y, scale_z);
}

//GETTERS
//position
float Model3D::getPos_X() {
    return this->pos_x;
}

float Model3D::getPos_Y() {
    return this->pos_y;
}

float Model3D::getPos_Z() {
    return this->pos_z;
}

void Model3D::setPos_X(float pos_x) {
    this->pos_x = pos_x;
}

void Model3D::setPos_Y(float pos_y) {
    this->pos_y = pos_y;
}

void Model3D::setPos_Z(float pos_z) {
    this->pos_z = pos_z;
}
//SCALE
float Model3D::getScale_X() {
    return this->scale_x;
}

float Model3D::getScale_Y() {
    return this->scale_y;
}

float Model3D::getScale_Z() {
    return this->scale_z;
}

void Model3D::setScale_X(float scale_x)
{
    this->scale_x = scale_x;
}

void Model3D::setScale_Y(float scale_y)
{
    this->scale_y = scale_y;
}

void Model3D::setScale_Z(float scale_z)
{
    this->scale_z = scale_z;
}

//AXIS
float Model3D::getAxis_X() {
    return this->axis_x;
}

float Model3D::getAxis_Y() {
    return this->axis_y;
}

float Model3D::getAxis_Z() {
    return this->axis_z;
}

void Model3D::setAxis_X(float axis_x) {
    this->axis_x = axis_x;
}

void Model3D::setAxis_Y(float axis_y) {
    this->axis_y = axis_y;
}

void Model3D::setAxis_Z(float axis_z) {
    this->axis_z = axis_z;
}

//THETA
float Model3D::getTheta_X() {
    return this->theta_x;
}

float Model3D::getTheta_Y() {
    return this->theta_y;
}

float Model3D::getTheta_Z() {
    return this->theta_z;
}

void Model3D::setTheta_X(float theta_x) {
    this->theta_x = theta_x;
}

void Model3D::setTheta_Y(float theta_y) {
    this->theta_y = theta_y;
}

void Model3D::setTheta_Z(float theta_z) {
    this->theta_z = theta_z;
}