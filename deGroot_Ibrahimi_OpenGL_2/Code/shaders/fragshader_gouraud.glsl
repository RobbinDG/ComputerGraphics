#version 330 core

// Define constants
#define M_PI 3.141593

uniform sampler2D samplerUniform;

// The inputs to the fragment shader
in vec3 Ia;
in vec3 Id;
in vec3 Is;
in vec2 textureCoords;

// The output of the fragment shader
out vec4 fColor;

void main()
{
    // Apply texture
    vec4 textureColor = texture(samplerUniform, textureCoords);
    vec3 color = Ia * textureColor.rgb + Id * textureColor.rgb + Is;

    // Set color
    fColor = vec4(color, 1);
}
