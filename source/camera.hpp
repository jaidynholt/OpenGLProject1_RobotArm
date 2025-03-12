#ifndef camera_hpp
#define camera_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera {
public:
    camera();   // default constructor - uses default values
    camera(bool isOrthographic, float cameraSpeed);   // default constructor - just use default perspective matrix values
    // default destructor

    void setCameraSpeed(float newSpeed);
    void makeOrthographic(bool isOrthographic);

    void moveCamera(int key, float deltaTime);

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

private:
    // matrices
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    // camera info
    glm::vec3 cameraPosition;  // the current camera position
    glm::vec3 cameraUp;  // the current camera position's up vector
    glm::vec3 cameraRight; // the current camera position's right vector
    float cameraSpeed;  // how fast the camera moves on input

    // internal method for updating view matrix after moving camera
    void setViewMatrix();
};

#endif
