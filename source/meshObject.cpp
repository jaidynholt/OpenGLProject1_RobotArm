#include "meshObject.hpp"

// Initialize static members
int meshObject::nextId = 1;
std::map<int, meshObject*> meshObject::meshObjectMap;
light meshObject::light1 = light(
    glm::vec3(4, 8, 4),
    glm::vec3(1.0, 0.3, 0.3),
    glm::vec3(1.0, 0.3, 0.3) * glm::vec3(0.3, 0.3, 0.3),
    6.0f,
    100.0f
);
light meshObject::light2 = light(
    glm::vec3(-4, 8, 4),
    glm::vec3(0.3, 0.8, 0.8),
    glm::vec3(0.3, 0.8, 0.8) * glm::vec3(0.3, 0.3, 0.3),
    6.0f,
    100.0f
);

// default constructor that has no model, just an empty transform
meshObject::meshObject() : id(nextId++) {
    // Add this object to the map
    meshObjectMap[id] = this;
    isProjectile = false;
    isEmpty = true;

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    pivot = glm::vec3(0.0f, 0.0f, 0.0f);

    // initialize the parent to null
    parent = nullptr;
}

meshObject::meshObject(const char* objFilePath) : id(nextId++) { // Assign current value of nextId to id and increment it
    // Add this object to the map
    meshObjectMap[id] = this;
    isProjectile = false;
    isEmpty = false;

    // vectors for storing the obj's vertices, uvs, and normals
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    // load the data from file into those vectors using objloader
    bool success = loadOBJ(objFilePath, vertices, uvs, normals, indices);
    if (!success) { std::cerr << "Failed to load obj file" << std::endl; }

    numIndices = (GLsizei)indices.size();

    // VAO
    glGenVertexArrays(1, &VAO); // generate
    glBindVertexArray(VAO); // bind VAO

    // VBO - vertices
    glGenBuffers(1, &VBO);  // generate
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);  // send data to the buffer
    // configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // vertex attribute 0 (position)
    glEnableVertexAttribArray(0);   // enable attribute 0

    // VBO-UVs
    glGenBuffers(1, &VBO_UVs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    // configure vertex attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // vertex attribute 1 (uvs)
    glEnableVertexAttribArray(1);   // enable attribute 1

    // VBO - normals
    glGenBuffers(1, &VBO_Normals);  // generate
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals); // bind
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);  // send data to the buffer
    // configure vertex attributes
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // vertex attribute 2 (normals)
    glEnableVertexAttribArray(2);   // enable attribute 2

    // EBO
    glGenBuffers(1, &EBO);    // generate
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::vec3), &indices[0], GL_STATIC_DRAW);    // send data to the buffer

    // unbind
    glBindVertexArray(0);   // unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // unbind EBO
    
    // Load and compile shaders
    shaderProgram = LoadShaders("../source/shaders/meshVertexShader.glsl", "../source/shaders/meshFragmentShader.glsl");
    pickingShaderProgram = LoadShaders("../source/shaders/pickingvertexShader.glsl", "../source/shaders/pickingFragmentShader.glsl");

    // Load the texture
    Texture = loadDDS("../source/textures/robotTextureImage.DDS");
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // set to draw as wireframe

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    pivot = glm::vec3(0.0f, 0.0f, 0.0f);

    // initialize the parent to null
    parent = nullptr;
}

meshObject::~meshObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    // clean up pointers to this
    // remove pointer from parent
    // remove this' pointers to parent and children
    if (isProjectile) {
        parent->children.pop_back();
        std::cout << parent->children.size() << std::endl;
    }
    for (meshObject* child : children) {
        delete child;
    }
    meshObjectMap.erase(id);    // Remove this object from the map
    std::cout << id << "destroyed" << std::endl;
}

//TODO: P1bTask5 - Modify to accept lighiting info as arguement.
void meshObject::draw(const glm::mat4& view, const glm::mat4& projection, float deltaTime, int selectedID) {
    if (!isEmpty) {
        glUseProgram(shaderProgram);

        // Compute the MVP matrix
        glm::mat4 model = getWorldSpaceMatrix();
        if (isProjectile && !hasLanded) { projectileMotion(deltaTime); }

        glm::mat4 MVP = projection * view * model;
        // get handles to uniforms in shader program and send values
        GLuint MVPID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint MID = glGetUniformLocation(shaderProgram, "ModelMatrix");
        GLuint VID = glGetUniformLocation(shaderProgram, "ViewMatrix");
        glUniformMatrix4fv(MVPID, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniformMatrix4fv(MID, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(VID, 1, GL_FALSE, glm::value_ptr(view));

        // send texture to shader program: Set our "texSampler" sampler to use Texture Unit 0
        GLuint TextureID = glGetUniformLocation(shaderProgram, "texSampler");
        glUniform1i(TextureID, 0);

        // load lights into shader program
        GLuint LightID = glGetUniformLocation(shaderProgram, "light1PositionWorldSpace");
        GLuint LightDColorID = glGetUniformLocation(shaderProgram, "light1DiffuseColor");
        GLuint LightAColorID = glGetUniformLocation(shaderProgram, "light1AmbientColor");
        GLuint LightSColorID = glGetUniformLocation(shaderProgram, "light1SpecularColor");
        GLuint LightShininessID = glGetUniformLocation(shaderProgram, "light1Shininess");
        GLuint LightPowerID = glGetUniformLocation(shaderProgram, "light1Power");
        glUniform3f(LightID, light1.getPosition().x, light1.getPosition().y, light1.getPosition().z);
        glUniform3f(LightDColorID, light1.getDiffuseColor().x, light1.getDiffuseColor().y, light1.getDiffuseColor().z);
        glUniform3f(LightAColorID, light1.getAmbientColor().x, light1.getAmbientColor().y, light1.getAmbientColor().z);
        glUniform3f(LightSColorID, light1.getSpecularColor().x, light1.getSpecularColor().y, light1.getSpecularColor().z);
        glUniform1f(LightShininessID, light1.getPower());
        glUniform1f(LightPowerID, light1.getPower());
        LightID = glGetUniformLocation(shaderProgram, "light2PositionWorldSpace");
        LightDColorID = glGetUniformLocation(shaderProgram, "light2DiffuseColor");
        LightAColorID = glGetUniformLocation(shaderProgram, "light2AmbientColor");
        LightSColorID = glGetUniformLocation(shaderProgram, "light2SpecularColor");
        LightShininessID = glGetUniformLocation(shaderProgram, "light2Shininess");
        LightPowerID = glGetUniformLocation(shaderProgram, "light2Power");
        glUniform3f(LightID, light2.getPosition().x, light2.getPosition().y, light2.getPosition().z);
        glUniform3f(LightDColorID, light2.getDiffuseColor().x, light2.getDiffuseColor().y, light2.getDiffuseColor().z);
        glUniform3f(LightAColorID, light2.getAmbientColor().x, light2.getAmbientColor().y, light2.getAmbientColor().z);
        glUniform3f(LightSColorID, light2.getSpecularColor().x, light2.getSpecularColor().y, light2.getSpecularColor().z);
        glUniform1f(LightShininessID, light2.getPower());
        glUniform1f(LightPowerID, light2.getPower());


        // draw selected obj in brighter color
        GLuint boolID = glGetUniformLocation(shaderProgram, "isSelected");
        // if the selectedID matches this id (OR special case for the base empty)
        if ((selectedID == id) || (selectedID == 1 && id == 2))
        {
            glUniform1i(boolID, 1);
        }
        else {
            glUniform1i(boolID, 0);
        }

        // Draw the object
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0); // Unbind the shader program
    }

    // call draw on all children
    for(meshObject* child : children) {
        child->draw(view, projection, deltaTime, selectedID);
    }
}

void meshObject::translate(const glm::vec3& translation) {
    position += translation;        // Apply translation
    computeModelMatrix(); // compute new model matrix
}

void meshObject::rotate(float angle, const glm::vec3& axis) {
    rotation *= glm::angleAxis(glm::radians(angle), axis);  // Apply rotation
    computeModelMatrix(); // compute new model matrix
}

glm::mat4 meshObject::computeModelMatrix() {
    // get matrices for the transformations    
    modelMatrix = glm::translate(glm::mat4(1.0f), -pivot); // translate so pivot at origin
    modelMatrix = glm::mat4_cast(rotation) * modelMatrix;   // rotate
    modelMatrix = modelMatrix * glm::translate(glm::mat4(1.0f), position);    // translate to position

    return modelMatrix;
}

void meshObject::drawPicking(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(pickingShaderProgram); // Use the picking shader

    //Send the value of the id variable to the shader as a uniform
    GLuint objectID = glGetUniformLocation(pickingShaderProgram, "objectID");
    glUniform1f(objectID, (float)id);

    // send MVP matrix to shader
    glm::mat4 MVP = projection * view * getWorldSpaceMatrix();
    GLuint matrixID = glGetUniformLocation(pickingShaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0); // Unbind the shader program

    // call draw on all children
    /*for (meshObject* child : children) {
        child->drawPicking(view, projection);
    }*/
}

meshObject* meshObject::getMeshObjectById(int id) {
    // Look up the object by ID in the map
    auto it = meshObjectMap.find(id);
    if (it != meshObjectMap.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if ID not found
}

meshObject* meshObject::createChild(const char* objFilePath, const glm::vec3& pivot) {
    meshObject* child = new meshObject(objFilePath);    // create child
    // set pivot and parent info
    child->parent = this;
    child->pivot = pivot;

    child->computeModelMatrix(); // child's model matrix starts at parent's

    // add to list of children
    this->children.push_back(child);   
    std::cout << "child created of " << id << ": " << child->getId() << std::endl;
    return child;
}

glm::mat4 meshObject::getWorldSpaceMatrix() {
    if (parent) return parent->getWorldSpaceMatrix() * modelMatrix;
    return modelMatrix;
}

void meshObject::shootProjectile() {
    // create new mesh
    meshObject* projectile = this->createChild("../source/models/JointModel.obj", glm::vec3(0.0f, 0.0f, 0.0f));

    // get matrix to transform to tip, relative to this
    glm::mat4 TipMatrix = meshObject::getMeshObjectById(5)->getWorldSpaceMatrix();  // matrix to transform from world to tip
    TipMatrix = glm::translate(TipMatrix, glm::vec3(0.0f, 2.0f, 0.0f)); // aditional 2 units to tip
    //TipMatrix = glm::translate(TipMatrix, -position);   // reverse translate from base so it's relative to base

    // get point at tip and point at base; subtract to get vector to tip from base
    glm::vec4 TipPosition = TipMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 BasePosition = modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 vectorToTip = glm::vec3(TipPosition - BasePosition);  // subtract the points to get the vector

    // print outs
    std::cout << TipPosition.x << ", " << TipPosition.y << ", " << TipPosition.z << std::endl;
    std::cout << BasePosition.x << ", " << BasePosition.y << ", " << BasePosition.z << std::endl;
    std::cout << vectorToTip.x << ", " << vectorToTip.y << ", " << vectorToTip.z << std::endl;

    projectile->translate(vectorToTip);   // translate projectile to tip of arm2

    // set up info as a projectile
    projectile->isProjectile = true;

    // get launch vector relative to this by taking vector between tip position and arm2 position
    glm::mat4 arm2Matrix = meshObject::getMeshObjectById(5)->getWorldSpaceMatrix();
    //arm2Matrix = glm::translate(arm2Matrix, -position);   // translate so relative to base
    glm::vec4 arm2Position = arm2Matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    projectile->projectileForceVector = glm::vec3(TipPosition - arm2Position);
    projectile->projectileForceVector = glm::normalize(projectile->projectileForceVector) * 4.0f; // power as the length of arm2 (which is 4)

    // print out
    std::cout << projectile->projectileForceVector.x << ", " << projectile->projectileForceVector.y << ", " << projectile->projectileForceVector.z << std::endl;
}

void meshObject::projectileMotion(float deltaTime) {
    // apply gravity vector downward
    // gravity force = m*a = 1.0*9.8
    glm::vec3 gravity = glm::vec3(0.0f, 1.0f, 0.0f) * -9.8f * deltaTime;
    projectileForceVector += gravity;

    // translate the projectile according to the forces and deltaTime
    modelMatrix = glm::translate(modelMatrix, projectileForceVector * deltaTime);

    // check if the projectile has reached the grid
    if ((modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).y <= 0.0f) {
        // translate robot to the position; get the world positions of projectile and robot, then translate
        glm::vec4 projectilePosition = getWorldSpaceMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 parentPosition = parent->getWorldSpaceMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec3 moveVector = glm::vec3(projectilePosition - parentPosition);

        parent->translate(moveVector);

        this->hasLanded = true;
        // destroy projectile
        this->~meshObject();
    }
}