#version 330 core

// Define constants
#define M_PI 3.141593

uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;
uniform sampler2D samplerUniform;

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 lightPos;
in vec3 vertexNormal;
in vec3 vertexCoords;
in vec2 textureCoords;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    vec3 lightPos = vec3(modelViewTransform * vec4(100.0, 100.0, 150.0, 1.0));

    vec3 N = normalize(transNorms * vertexNormal);
    vec3 L = normalize(lightPos - vertexCoords);
    vec3 R = normalize(2 * dot(N, L) * N - L);
    vec3 V = 0 - vertexCoords;

    int p = 1; // TODO

    vec4 textureColor = texture(samplerUniform, textureCoords);

    vec3 Ia = vec3(0.5, 0.5, 0.5); // TODO
    vec3 Id = max(0.0, dot(N, L)) * vec3(textureColor);
    vec3 Is = pow(max(0.0, dot(R, V)), p) * vec3(1, 1, 1);

    vec3 color = Ia + Id + Is;

    fColor = vec4(color, 1);
}
