#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertexCoords_in;
layout (location = 1) in vec3 vertexNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;

// Specify the output of the vertex stage
out vec3 vertexNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertexCoords_in, 1.0);
    vertexNormal = transNorms * vertexNormal_in;
}
