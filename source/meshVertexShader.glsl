#version 330 core

// Input vertex attributes (from VBO)
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal; // Normals

//output data: interpolated for each fragment
out vec2 UV;
out vec3 positionWorldSpace;
out vec3 normalCameraSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 light1DirectionCameraSpace;
out vec3 light2DirectionCameraSpace;

// Uniform (constant) values
uniform mat4 MVP; // Combined Model-View-Projection matrix
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform vec3 light1PositionWorldSpace;
uniform vec3 light2PositionWorldSpace;

void main() {
    // vertex position, clip space
    gl_Position = MVP * vec4(position, 1.0);

    // vertex position, world space
	positionWorldSpace = (ModelMatrix * vec4(position, 1.0)).xyz;
	
	// Vector from the vertex to camera, camera space.
	// In camera space, camera at origin (0,0,0).
	vec3 vertexCameraSpace = ( ViewMatrix * ModelMatrix * vec4(position, 1.0)).xyz;
	eyeDirectionCameraSpace = vec3(0,0,0) - vertexCameraSpace;

	// Vector from vertex to light, camera space.
	vec3 light1PositionCameraSpace = ( ViewMatrix * vec4(light1PositionWorldSpace, 1.0)).xyz;
	light1DirectionCameraSpace = light1PositionCameraSpace + eyeDirectionCameraSpace;

	vec3 light2PositionCameraSpace = ( ViewMatrix * vec4(light2PositionWorldSpace, 1.0)).xyz;
	light2DirectionCameraSpace = light2PositionCameraSpace + eyeDirectionCameraSpace;
	
	// Normal of vertex, camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	normalCameraSpace = ( ViewMatrix * ModelMatrix * vec4(normal,0)).xyz;
	
	// UV of the vertex.
	UV = uv;
}
