#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertexCoords_in;
layout (location = 1) in vec3 vertexNormal_in;
layout (location = 2) in vec2 textureCoords_in;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;

//uniform vec3 lightPos;
//uniform vec3 lightColor;
//uniform vec3 lightIntensity;
//uniform vec3 material;       // 0 = Ambient coeff, 1 = Diffuse coeff, 2 = Specular coeff

// Specify the output of the vertex stage
out vec3 color;
out vec2 textureCoords;

void main()
{
    vec3 Ia, Id, Is;

    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertexCoords_in, 1.0);

    vec3 lightPos = vec3(modelViewTransform * vec4(100.0, 100.0, 150.0, 1.0));

    vec3 N = normalize(transNorms * vertexNormal_in);
    vec3 L = normalize(lightPos - vertexCoords_in);
    vec3 R = normalize(2 * dot(N, L) * N - L);
    vec3 V = 0 - vertexCoords_in;

    int p = 1; // TODO

    vec3 material = vec3(1, 1, 1);
    Ia = vec3(0.5, 0.5, 0.5); // TODO
    Id = max(0.0, dot(N, L)) * material[1] * vec3(1, 1, 1);
    Is = pow(max(0.0, dot(R, V)), p) * material[2] * vec3(1, 1, 1);

    color = Ia + Id + Is;
    textureCoords = textureCoords_in;
}
