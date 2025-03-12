#ifndef light_hpp
#define light_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class light {
public:
    light();   // default constructor - uses default values
    light(
        glm::vec3 position,
        glm::vec3 diffuseColor,
        glm::vec3 ambientColor,
        float shininess,
        float power
    );

    // getters
    glm::vec3 getPosition();
    glm::vec3 getDiffuseColor();
    glm::vec3 getAmbientColor();
    glm::vec3 getSpecularColor();
    float getShininess();
    float getPower();

private:
    glm::vec3 position;
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    float shininess;
    float power;
};

#endif
