#version 460 core

uniform vec3 lightColor;

out vec4 frag_colour;

void main()
{
    frag_colour = vec4(lightColor, 1.0);
}