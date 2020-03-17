#version 330 core

// Define constants
#define M_PI 3.141593

uniform mat4 modelViewTransform;
uniform mat4 projectionTransform;
uniform mat3 transNorms;
uniform vec3 material; // (0.3 , 1 , 0.2)
uniform sampler2D samplerUniform;

// The inputs to the fragment shader
in vec3 lightPos;
in vec3 vertexNormal;
in vec3 vertexCoords;
in vec2 textureCoords;

// The output of the fragment shader
out vec4 fColor;

void main()
{
    vec3 lightPos = vec3(modelViewTransform * vec4(80.0, 80.0, 100.0, 1.0));

    // Calculate the vectors needed for the Phong illumination model
    vec3 N = normalize(transNorms * vertexNormal);
    vec3 L = normalize(lightPos - vertexCoords);
    vec3 R = normalize(2 * dot(N, L) * N - L);
    vec3 V = normalize(0 - vertexCoords);

    int p = 1;

    vec4 textureColor = texture(samplerUniform, textureCoords);

    // Calculate the components of the Phong illumination model
    vec3 Ia = material[0] * textureColor.rgb;
    vec3 Id = max(0.0, dot(N, L)) * material[1] * textureColor.rgb;
    vec3 Is = pow(max(0.0, dot(R, V)), p) * material[2] * vec3(1, 1, 1);

    vec3 color = Ia + Id + Is;

    fColor = vec4(color, 1);
}
