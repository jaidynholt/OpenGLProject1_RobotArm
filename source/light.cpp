#include "light.hpp"

light::light() {
    position = glm::vec3(0.0, 0.0, 0.0f);
    diffuseColor = glm::vec3(1.0, 1.0, 1.0f);
    ambientColor = glm::vec3(1.0, 1.0, 1.0f);
    specularColor = glm::vec3(1.0, 1.0, 1.0f);
    shininess = 5.0f;
    power = 50.0f;
}
light::light(
    glm::vec3 position,
    glm::vec3 diffuseColor,
    glm::vec3 ambientColor,
    float shininess,
    float power
) {
    this->position = position;
    this->diffuseColor = diffuseColor;
    this->ambientColor = ambientColor;
    this->specularColor = diffuseColor * 0.7f;
    this->shininess = shininess;
    this->power = power;
}

// getters
glm::vec3 light::getPosition() {
    return position;
}
glm::vec3 light::getDiffuseColor() {
    return diffuseColor;
}
glm::vec3 light::getAmbientColor() {
    return ambientColor;
}
glm::vec3 light::getSpecularColor() {
    return specularColor;
}
float light::getShininess() {
    return shininess;
}
float light::getPower() {
    return power;
}