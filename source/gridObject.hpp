#ifndef gridObject_hpp
#define gridObject_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <common/shader.hpp>

class gridObject {
public:
    gridObject();   // constructor
    ~gridObject();  // destructor

    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO, EBO;   // handles
    GLuint shaderProgram;   // shaders (compiled in constructor)
    glm::mat4 modelMatrix;  // Model Matrix (M): object -> world
    GLsizei numIndices; // number of indices (in EBO)
};

#endif
