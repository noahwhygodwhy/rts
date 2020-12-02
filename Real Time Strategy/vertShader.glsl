#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

out vec2 texCoords;
out vec2 coords;
void main()
{

    coords = aPos;
    texCoords = aUV;
    gl_Position = projection*view*transform*vec4(aPos, 0.0, 1.0);
}

