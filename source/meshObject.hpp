#ifndef meshObject_hpp
#define meshObject_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include <iostream>
#include "OpenGLTutorial/objloader.hpp"
#include "OpenGLTutorial/texture.hpp"
#include "OpenGLTutorial/shader.hpp"
#include "light.hpp"

class meshObject {
public:
    // constructor that takes all the object's filepath
    meshObject();
    meshObject(const char* objFilePath);
    ~meshObject();  // destructor

    void draw(const glm::mat4& view, const glm::mat4& projection, float deltaTime, int selectedID);
    void drawPicking(const glm::mat4& view, const glm::mat4& projection);

    void translate(const glm::vec3& translation); // Translate the object
    void rotate(float angle, const glm::vec3& axis); // Rotate the object

    int getId() const { return id; } // Getter for the ID

    void shootProjectile();
    void projectileMotion(float deltaTime);
    
    static meshObject* getMeshObjectById(int id); // Retrieve object by ID
    
    // children
    std::vector<meshObject*> children;
    meshObject* createChild(const char* objFilePath, const glm::vec3& pivot);

    // parent
    meshObject* parent;

    // if this is a projectile, every frame it moves under gravity and force
    bool isProjectile;
    bool isEmpty;

private:
    GLuint VAO, VBO, VBO_UVs, VBO_Normals, EBO;
    GLsizei numIndices;

    GLuint shaderProgram;
    GLuint pickingShaderProgram;

    GLuint Texture;

    glm::mat4 modelMatrix;

    glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 projectileForceVector;
    bool hasLanded = false;

    glm::mat4 computeModelMatrix();
    glm::mat4 getWorldSpaceMatrix();

    int id;            // ID for this specific object    

    static int nextId; // Static counter for unique IDs
    static std::map<int, meshObject*> meshObjectMap; // Static map of ID to Object

    // lights
    static light light1;
    static light light2;
};

#endif
