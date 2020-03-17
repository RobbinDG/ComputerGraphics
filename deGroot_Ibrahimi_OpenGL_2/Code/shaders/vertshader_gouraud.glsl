#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertexCoords_in;
layout (location = 1) in vec3 vertexNormal_in;
layout (location = 2) in vec2 textureCoords_in;

uniform sampler2D samplerUniform;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;
uniform vec3 material; // (0.3 , 1 , 0.2)

// Specify the output of the vertex stage
out vec3 Ia;
out vec3 Id;
out vec3 Is;
out vec2 textureCoords;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertexCoords_in, 1.0);

    vec3 lightPos = vec3(modelViewTransform * vec4(80.0, 80.0, 100.0, 1.0));

    // Calculate the vectors needed for the Phong illumination model
    vec3 N = normalize(transNorms * vertexNormal_in);
    vec3 L = normalize(lightPos - vertexCoords_in);
    vec3 R = normalize(2 * dot(N, L) * N - L);
    vec3 V = normalize(0 - vertexCoords_in);

    int p = 1;

    // Calculate the components of the Phong illumination model
    Ia = material[0] * vec3(1, 1, 1);
    Id = max(0.0, dot(N, L)) * material[1] * vec3(1, 1, 1);
    Is = pow(max(0.0, dot(R, V)), p) * material[2] * vec3(1, 1, 1);

    textureCoords = textureCoords_in;
}
