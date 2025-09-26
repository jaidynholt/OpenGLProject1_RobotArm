# Robot Arm

## Project Description
**OpenGL Project 1: CAP4730 Computer Graphics  
Spring 2025**

An interactive robot arm with a single joint which moves and rotates on a XZ plane.<br>
My first project with OpenGL focused on affine hierarchical transformations, drawing and lighting with shaders.

### Camera Movement
The camera is fixed looking at the origin and moves around the origin in a sphere.
- **Select camera**: press `c` key
- **Move camera**: press `←` `→` `↑` `↓` keys

### Robot Movement
The robot can move and rotate on the XZ plane.
- **Select robot base to move**: press `t` key
- **Move robot base**: press `←` `→` `↑` `↓` keys
- **Select robot base to rotate**: `L mouse click` on it _or_ press `r` key
- **Rotate robot base**: press `←` `→` keys

### Arm Movement
The two arm segments can rotate.
- **Select arm segment 1**: `L mouse click` on it _or_ press `1` key
- **Select arm segment 2**: `L mouse click` on it _or_ press `2` key
- **Rotate arm segment**: press `↑` `↓` keys

### Projectile Teleportation
The robot can shoot a projectile out of its arm (with force equal to its length) and teleport to where it lands.
- **Shoot projectile**: press `s` key

## Build
The project can be built using [_CMake_](https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/#build-and-test).

## Libraries / External Resources
The project uses the following libraries:
- **GLFW**: Open Source library for OpenGL, OpenGL ES, and Vulkan for creating windows, contexts and surfaces, receiving input and events
- **OpenGL Extension Wrangler Library (GLEW)**: open-source C/C++ extension loading library
- **OpenGL Mathematics (GLM)**: C++ mathematics library for graphics software based on GLSL specs

The project was built upon a minimal starter code base (see below) from the course and uses resources from [Learn OpenGL](https://www.opengl-tutorial.org/) for loading shaders, loading textures, and loading OBJ models. Scripts in the folder `source/OpenGLTutorial` are these external scripts.

## CAP4730 Computer Graphics Project Description
[Part 1: Affine transformations, drawing, input](https://www.cise.ufl.edu/research/SurfLab/gfxNotes/cap4730/HW/Prj1/hw1a.html)<br>
[Part 2: Hierarchical transformations, lighting, picking, projectile teleport](https://www.cise.ufl.edu/research/SurfLab/gfxNotes/cap4730/HW/Prj1/hw1b.html)
