#version 330 core

in vec3 interpolatedColor;
out vec4 pixelRGBA;

void main()
{
    pixelRGBA = vec4(interpolatedColor, 1);
}
