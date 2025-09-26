cmake_minimum_required (VERSION 3.5)
project (RobotArm)


# set C++17 standard
set(CMAKE_CXX_STANDARD 17)


# find OpenGL dependecies
find_package(OpenGL REQUIRED)


# prevent build inside root directory
if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Build directory is same as root! Select a subdirectory \root instead" )
endif()


# warnings for spaces in directory names
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "WARNING: the source directory contains spaces. May result in compile issues." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "WARNING: the source directory contains spaces. May result in compile issues." )
endif()


# Compile external dependencies 
add_subdirectory(external)


# Enable use of legacy CMake include_directories (global)
cmake_policy(SET CMP0026 OLD)

# create variable ALL_LIBS containing all the library dependencies
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

# add compile time macros for static linking and other stuff
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DGLEW_STATIC
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)


add_executable(robot-arm
	source/main.cpp
	source/meshObject.cpp
	source/meshObject.hpp
	source/gridObject.cpp
	source/gridObject.hpp
	source/camera.cpp
	source/camera.hpp
	source/light.cpp
	source/light.hpp

	source/OpenGLTutorial/shader.cpp
	source/OpenGLTutorial/shader.hpp
	source/OpenGLTutorial/texture.cpp
	source/OpenGLTutorial/texture.hpp
	source/OpenGLTutorial/objloader.cpp
	source/OpenGLTutorial/objloader.hpp
	
	source/shaders/meshVertexShader.glsl
	source/shaders/meshFragmentShader.glsl
	source/shaders/gridVertexShader.glsl
	source/shaders/gridFragmentShader.glsl
	source/shaders/pickingVertexShader.glsl
	source/shaders/pickingFragmentShader.glsl
)


# include directories to compiler header search path
include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	.
)


# link the executable to the libraries
target_link_libraries(robot-arm PRIVATE ${ALL_LIBS})


# create Visual Studio / XCode filter groups
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/shaders/.*" )
SOURCE_GROUP(OpenGLTutorialResources REGULAR_EXPRESSION ".*/OpenGLTutorial/.*" )


# move executable to the source directory if not in XCode
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
	add_custom_command(
   		TARGET robot-arm POST_BUILD
   		COMMAND ${CMAKE_COMMAND} -E copy
		"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/robot-arm${CMAKE_EXECUTABLE_SUFFIX}"
		"${CMAKE_CURRENT_SOURCE_DIR}/source/"
	)
endif ()