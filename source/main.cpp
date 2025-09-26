// Project 1A
// Jaidyn Holt
// CAP4730 Spring 2025

#include <GL/glew.h>    // OpenGL Extension Wrangler Library (GLEW): open-source C/C++ extension loading library
#include <GLFW/glfw3.h> // library for creating windows, contexts and surfaces, receiving input and events

#include <glm/glm.hpp>  // OpenGL Mathematics (GLM): C++ mathematics library for graphics software based on GLSL specs
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "camera.hpp"
#include "meshObject.hpp"
#include "gridObject.hpp"

// Function prototypes
int initWindow(void);   // Initialize GLFW and create a window
static void mouseCallback(GLFWwindow*, int, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
int getPickedId();

const GLuint windowWidth = 1024, windowHeight = 768;    // define constant window size
GLFWwindow* window; // reference to window
camera* cam;

int currSelected = 0;   // object that is currently selected
// 0:camera
// 1: base empty
// 2: base obj
// 3: arm1 obj
// 4: joint
// 5: arm2 obj
// 6+: projectiles

bool isPicking = false; // for when the picking pfunction is active, do not draw


int main() {
    // initialize window
    if (initWindow() != 0) return -1;

    cam = new camera(); // initialize camera object
    glm::mat4 projectionMatrix = cam->getProjectionMatrix();
    glm::mat4 viewMatrix;

    gridObject grid;    // instantiate the grid object (which also compiles its shaders)
        
    // Create each robot arm piece by creating instances class meshObject
    meshObject robot;
    robot.createChild("../source/models/BaseModel.obj", glm::vec3(0.0f, 0.0f, 0.0f));
    robot.children[0]->createChild("../source/models/Arm1Model.obj", glm::vec3(0.0f, -2.0f, 0.0f));
    robot.children[0]->children[0]->createChild("../source/models/JointModel.obj", glm::vec3(0.0f, -2.0f, 0.0f));
    robot.children[0]->children[0]->children[0]->createChild("../source/models/Arm2Model.obj", glm::vec3(0.0f, -2.0f, 0.0f));

    // transform the arms
    //meshObject::getMeshObjectById(2)->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));   // rotate base 45 about y
    meshObject::getMeshObjectById(3)->rotate(20.0f, glm::vec3(1.0f, 0.0f, 0.0f));   // rotate arm1 30 about x
    meshObject::getMeshObjectById(5)->rotate(85.0f, glm::vec3(1.0f, 0.0f, 0.0f));   // rotate arm1 30 about x
        
    // frames per second metric:
    double lastframeTime = glfwGetTime();    // get current time
    double deltaTime = 0;
    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    do {
        // TIMING
        // calculating delta time
        deltaTime = glfwGetTime() - lastframeTime;
        lastframeTime = glfwGetTime();
        // calculating speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
            // printf and reset
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // HANDLE MOVEMENT INPUTS
        // based on currSelected, make transformations and set shaders
        switch (currSelected) {
        case 0:
            // if keys are pressed, move+rotate the camera
            // multiply by delta time to be independent of frame rate
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                cam->moveCamera(GLFW_KEY_UP, (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                cam->moveCamera(GLFW_KEY_DOWN, (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                cam->moveCamera(GLFW_KEY_LEFT, (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                cam->moveCamera(GLFW_KEY_RIGHT, (float)deltaTime);
            }
            viewMatrix = cam->getViewMatrix();
            break;
        case 1:
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                robot.translate(glm::vec3(0.0f, 0.0f, 1.0f) * (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                robot.translate(-glm::vec3(0.0f, 0.0f, 1.0f) * (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                robot.translate(-glm::vec3(1.0f, 0.0f, 0.0f) * (float)deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                robot.translate(glm::vec3(1.0f, 0.0f, 0.0f) * (float)deltaTime);
            }
            break;
        case 2:
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                meshObject::getMeshObjectById(2)->rotate(-45.0f * (float)deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                meshObject::getMeshObjectById(2)->rotate(45.0f * (float)deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            break;
        case 3:
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                meshObject::getMeshObjectById(3)->rotate(-45.0f * (float)deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                meshObject::getMeshObjectById(3)->rotate(45.0f * (float)deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            break;
        case 5:
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                meshObject::getMeshObjectById(5)->rotate(-45.0f * (float)deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                meshObject::getMeshObjectById(5)->rotate(45.0f * (float)deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            break;
        }
        
        // DRAWING THE SCENE
        if (!isPicking) {
            glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // first clear buffers

            grid.draw(viewMatrix, projectionMatrix);    // draw grid
            robot.draw(viewMatrix, projectionMatrix, (float)deltaTime, currSelected);   // draw robot pieces; robot calls children mesh draws recursively
            //robot.drawPicking(viewMatrix, projectionMatrix);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }


    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}

// Initialize GLFW and create a window
int initWindow() {

    // initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // configure GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);    // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // specify OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // use core profile (no backward compatibility)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

    // create a window
    window = glfwCreateWindow(windowWidth, windowHeight, "Project1: Robot Arm", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    // make the context of the window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Set up inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);   // alow keyboard input
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    //glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);    // set cursor in middle of window
    glfwSetMouseButtonCallback(window, mouseCallback);  // callback for mouse
    glfwSetKeyCallback(window, key_callback);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    
    return 0;
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // get the picked id
        currSelected = getPickedId();
        std::cout << "Picked id: " << currSelected << std::endl;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // HANDLE KEY PRESS INPUTS
    // On key press set currSelected to the id of the robot piece to select.
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        // set to camera
        currSelected = 0;
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        // set to Base translation
        currSelected = 1;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        // set to Base rotation
        currSelected = 2;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        // set to arm1
        currSelected = 3;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        // set to arm2
        currSelected = 5;
    }
    // shooting mechanic
    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        meshObject::getMeshObjectById(1)->shootProjectile();
    }
}



int getPickedId(){
    isPicking = true;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // clear screen in dark blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw all robot arm pieces using drawPicking function
    meshObject::getMeshObjectById(2)->drawPicking(cam->getViewMatrix(), cam->getProjectionMatrix());
    meshObject::getMeshObjectById(3)->drawPicking(cam->getViewMatrix(), cam->getProjectionMatrix());
    meshObject::getMeshObjectById(5)->drawPicking(cam->getViewMatrix(), cam->getProjectionMatrix());

    glFlush();  // send pending draw commands to GPU immediately - SLOW
    glFinish(); // wait for draw commands to finish - SLOW
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //configure how glReadPixels will behave with respect to memory alignment
    
    // get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "x: " << xpos << ", y: " << ypos << std::endl;

    // set mouse position relative to window
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    xpos = xpos / width;
    ypos = ypos / height;
    std::cout << "x: " << xpos << ", y: " << ypos << std::endl;

    // set mouse position relative to buffer
    glfwGetFramebufferSize(window, &width, &height);
    xpos = xpos * width;
    ypos = ypos * height;

    //flip y to convert from window (start at top left) to openGL (start at bottom left)
    ypos = height - ypos;
    std::cout << "x: " << xpos << ", y: " << ypos << std::endl;

    // read pixel at mouse position into data
    // Note that y position has to be flipped as glfwGetCursorPos gives the cursor position relative to top left of the screen.
    //The read location must also be multiplied by (buffer size / windowSize) for some displays.
    unsigned char pixel[4];
    glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    printf("%f %f %f %f\n", pixel[0], pixel[1], pixel[2], pixel[3]);    

    // convert red channel back into id
    int pickedID = pixel[0];

    isPicking = false;
    
    return pickedID;
}
