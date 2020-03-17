#version 330 core

// Define constants
#define M_PI 3.141593

// The inputs to the fragment shader
in vec3 vertexNormal;

// The output of the fragment shader
out vec4 fColor;

void main()
{
    fColor = vec4((normalize(vertexNormal) + 1.0) / 2.0, 1.0);
}
