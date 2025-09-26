#version 330 core

// interpolated values from vertex shader
in vec2 UV;
in vec3 positionWorldSpace;
in vec3 normalCameraSpace;
in vec3 eyeDirectionCameraSpace;
in vec3 light1DirectionCameraSpace;
in vec3 light2DirectionCameraSpace;

// Output color
out vec3 color;

// uniforms (constant) values
uniform sampler2D texSampler;

uniform int isSelected;

uniform vec3 light1PositionWorldSpace;
uniform vec3 light2PositionWorldSpace;
uniform vec3 light1DiffuseColor;
uniform vec3 light2DiffuseColor;
uniform vec3 light1AmbientColor;
uniform vec3 light2AmbientColor;
uniform vec3 light1SpecularColor;
uniform vec3 light2SpecularColor;
uniform float light1Shininess; 
uniform float light2Shininess;
uniform float light1Power; 
uniform float light2Power;

void main() {
    // texture forms base color (diffuse)
    vec3 MaterialDiffuseColor = texture(texSampler, UV).rgb;

	// if selected, draw in a lighter shade
	if (isSelected == 1){
		MaterialDiffuseColor = MaterialDiffuseColor + vec3(0.8, 0.8, 0.8) * vec3(1.0, 1.0, 0.0);
	}

    // Distance to the light
	float distanceToLight1 = length(light1PositionWorldSpace - positionWorldSpace);
	float distanceToLight2 = length(light2PositionWorldSpace - positionWorldSpace);

    // Normal of fragment, camera space
	vec3 norm = normalize(normalCameraSpace);

	// Direction from fragment to light
	vec3 toLight1 = normalize(light1DirectionCameraSpace);
	vec3 toLight2 = normalize(light2DirectionCameraSpace);

	// Cosine of the angle between normal and light direction, clamped to [0,1]
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta1 = clamp(dot(norm, toLight1), 0, 1 );
	float cosTheta2 = clamp(dot(norm, toLight2), 0, 1 );

	// Eye vector (towards the camera)
	vec3 toEye = normalize(eyeDirectionCameraSpace);

	// Reflect vector (direction in which the triangle reflects the light)
	vec3 reflection1 = reflect(-toLight1, norm);
	vec3 reflection2 = reflect(-toLight2, norm);

	// Cosine of angle between the Eye vector and the Reflect vector, clamped to [0,1]
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha1 = clamp(dot(toEye, reflection1), 0, 1);
	float cosAlpha2 = clamp(dot(toEye, reflection2), 0, 1);
	
	// Ambient : simulates indirect lighting
	// Diffuse : "color" of the object
	// Specular : reflective highlight, like a mirror
	color =
		MaterialDiffuseColor * light1AmbientColor * light2AmbientColor +
		MaterialDiffuseColor * light1DiffuseColor * light1Power * cosTheta1 / (distanceToLight1 * distanceToLight1) +
		MaterialDiffuseColor * light2DiffuseColor * light2Power * cosTheta2 / (distanceToLight2 * distanceToLight2) +
		MaterialDiffuseColor * light1SpecularColor * light1Power * pow(cosAlpha1, light1Shininess) / (distanceToLight1 * distanceToLight1) +
		MaterialDiffuseColor * light2SpecularColor * light2Power * pow(cosAlpha2, light2Shininess) / (distanceToLight2 * distanceToLight2);
}

