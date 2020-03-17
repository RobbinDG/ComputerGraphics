#version 330 core

// Define constants
#define M_PI 3.141593
#define NUMBER_OF_WAVES 8

// Specify the input locations of attributes.
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormals_in;
layout (location = 2) in vec2 uvCoordinates_in;

// Specify the uniforms of the vertex shader.
uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform vec3 lightPosition;
uniform mat3 normalTransform;
uniform float amplitude[NUMBER_OF_WAVES];
uniform float frequency[NUMBER_OF_WAVES];
uniform float phase[NUMBER_OF_WAVES];
uniform float time;

out vec2 uv;
out vec3 N;
out vec3 V;
out vec3 materialColor;
out vec3 vertexCoord;
out vec3 relativeLightPosition;

// Calculates the normal based on the derivative of the water function f(u,t) = A * sin(2pi * f * u + t + phi)
float waveDU(int waveIdx, float u) {
    return 2 * M_PI * frequency[waveIdx] * amplitude[waveIdx] * cos(2.0 * M_PI * frequency[waveIdx] * u + time + phase[waveIdx]);
}

// Calculates the displacement of the z coordinate based on the derivative of the water function f(u,t) = A * sin(2pi * f * u + t + phi)
float waveHeight(int waveIdx, float u) {
    return amplitude[waveIdx] * sin(2.0 * M_PI * frequency[waveIdx] * u + time + phase[waveIdx]);
}

// Specify the output of the vertex stage.
void main()
{
    int waveIdx;
    float z = 0, dU = 0, dV = 0;

    // Get the z and dU component
    for (waveIdx = 0; waveIdx < NUMBER_OF_WAVES; waveIdx++) {
        z += waveHeight(waveIdx, uvCoordinates_in.x);
        dU += waveDU(waveIdx, uvCoordinates_in.x);
    }

    // Set current position
    vec3 currentVertex = vec3(vertCoordinates_in.x, vertCoordinates_in.y, z);
    gl_Position  = projectionTransform * modelViewTransform * vec4(currentVertex.x, currentVertex.y, z, 1.0F);

    float ss = smoothstep(-0.2F, 0.2F, z);
    vec3 min = vec3(0, 0, 1);   // Blue
    vec3 max = vec3(1, 1, 1);   // White

    // Out variables
    uv = uvCoordinates_in;
    N = normalTransform * normalize(vec3(-dU, -dV, 1.0F));
    V = vec3(inverse(projectionTransform)[3]);
    materialColor = mix(min, max, ss);
    vertexCoord = currentVertex;
    relativeLightPosition = vec3(modelViewTransform * vec4(lightPosition, 1.0F));
}
