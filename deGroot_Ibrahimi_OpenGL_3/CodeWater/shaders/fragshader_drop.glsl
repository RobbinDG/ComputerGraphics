#version 330 core

// Illumination model constants.
uniform vec4 material;
uniform vec3 lightColor;

in vec2 uv;
in vec3 N;
in vec3 V;
in vec3 materialColor;
in vec3 vertexCoord;
in vec3 relativeLightPosition;

out vec4 fColor;

void main()
{
//    The following were used for the previous parts of the assignment
//    fColor = vec4(uv[0], uv[1], 0.0F, 1.0F);
//    fColor = vec4(0.5 * N + vec3(0.5), 1.0F);
//    fColor = vec4(materialColor, 1.0F);
    vec3 lightPosition = normalize(relativeLightPosition - vertexCoord);
    vec3 L = normalize(lightPosition - vertexCoord);
    vec3 R = reflect(-L, N);
    vec3 Ia = materialColor * material.x;
    vec3 Id = (max(0.0, dot(L, N)) * lightColor) * materialColor * material.y;
    vec3 Is = (pow(max(0.0, dot(R, V - vertexCoord)), material.w) * lightColor) * material.z;

    fColor = vec4((Ia + Id + Is), 1.0);
}
