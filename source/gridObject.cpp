#include "gridObject.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

// constructor
gridObject::gridObject() { // Assign current value of nextId to id and increment it

    // define vertex info for the grid
    const GLfloat vertices[] = {
        // grid
        // Positions          // Colors
        -5.0, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,   // back left
        -5.0, 0.0f, -4.0f,  1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, -3.0f,  1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, -2.0f,  1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, -1.0f,  1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   // middle left
        -5.0, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, 2.0f,   1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, 3.0f,   1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, 4.0f,   1.0f, 1.0f, 1.0f,
        -5.0, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,    // front left
        5.0, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,   // back right
        5.0, 0.0f, -4.0f,   1.0f, 1.0f, 1.0f,
        5.0, 0.0f, -3.0f,   1.0f, 1.0f, 1.0f,
        5.0, 0.0f, -2.0f,   1.0f, 1.0f, 1.0f,
        5.0, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,
        5.0, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,
        5.0, 0.0f, 2.0f,    1.0f, 1.0f, 1.0f,
        5.0, 0.0f, 3.0f,    1.0f, 1.0f, 1.0f,
        5.0, 0.0f, 4.0f,    1.0f, 1.0f, 1.0f,
        5.0, 0.0f, 5.0f,    1.0f, 1.0f, 1.0f,    // front right
        -4.0f, 0.0f, -5.0f, 1.0f, 1.0f, 1.0f,  // back row left
        -3.0f, 0.0f, -5.0f, 1.0f, 1.0f, 1.0f,
        -2.0f, 0.0f, -5.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f, -5.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,  // back middle
        1.0f, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,
        2.0f, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,
        3.0f, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,
        4.0f, 0.0f, -5.0f,  1.0f, 1.0f, 1.0f,    // back row right
        -4.0f, 0.0f, 5.0f,  1.0f, 1.0f, 1.0f,  // front row left
        -3.0f, 0.0f, 5.0f,  1.0f, 1.0f, 1.0f,
        -2.0f, 0.0f, 5.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f, 5.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        2.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        3.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        4.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,    // front row right
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   // origin - white
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   // x axis - red
        5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // y axis - green
        0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // z axis - blue
        0.0f, 0.0f, 5.0f,   0.0f, 0.0f, 1.0f
    };    

    const GLuint indices[] = {
        // gridlines
        // parallel to x-axis (back->front)     // parallel to y-axis (left->right)
        0, 11,                                  0, 10,
        1, 12,                                  21, 30,
        2, 13,                                  22, 31,
        3, 14,                                  23, 32,
        4, 15,                                  24, 33,
        5, 38,                                  25, 38,
        6, 16,                                  26, 34,
        7, 17,                                  27, 35,
        8, 18,                                  28, 36,
        9, 19,                                  29, 37,
        10, 20,                                 11, 20,

        // axes
        39, 40, // x-axis
        41, 42, // y-axis
        43, 44  // z-axis
    };
    numIndices = sizeof(indices) / sizeof(indices[0]);

    // VAO
    glGenVertexArrays(1, &VAO); // generate
    glBindVertexArray(VAO); // bind VAO

    // VBO
    glGenBuffers(1, &VBO);  // generate
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // send data to the buffer

    // configure vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0); // vertex attribute 0 (position)
    glEnableVertexAttribArray(0);   // enable attribute 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // vertex attribute 1 (color)
    glEnableVertexAttribArray(1);   // enable attribute 1

    // EBO
    glGenBuffers(1, &EBO);    // generate
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // send data to the buffer

    // unbind
    glBindVertexArray(0);   // unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   // unbind EBO

    // Load and compile shaders
    shaderProgram = LoadShaders("../source/shaders/gridVertexShader.glsl", "../source/shaders/gridFragmentShader.glsl");

    // Initialize the model matrix
    modelMatrix = glm::mat4(1.0f);  // identity matrix
}

// destructor
gridObject::~gridObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
}

// given view and projection matrices, draw the grid object
void gridObject::draw(const glm::mat4& view, const glm::mat4& projection) {
    // specify use shader program
    glUseProgram(shaderProgram);

    // MVP matrix
    glm::mat4 MVP = projection * view * modelMatrix;    // compute MVP matrix (right to left)
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");   // get location of uniform variable in shader for MVP
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP)); // send MVP matrix to shader as uniform

    //TODO: P1bTask5 - Send lighting info to shader using uniform. May also need to send the model matrix seperatily as a uniform.

    // Draw the object
    glBindVertexArray(VAO); // bind VAO
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);   // draw grid
    glBindVertexArray(0);   // unbind VAO
}
