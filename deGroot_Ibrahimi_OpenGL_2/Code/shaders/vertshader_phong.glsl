#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;

// Specify the output of the vertex stage
out vec3 lightPos;
out vec3 vertNormal;
out vec3 vertCoordinates;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertCoordinates_in, 1.0);
    lightPos = vec3(modelViewTransform * vec4(100.0, 100.0, 150.0, 1.0));
    vertNormal = vertNormal_in;
    vertCoordinates = vertCoordinates_in;
}
