#version 330 core

layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 oColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(vVertex, 1);
    oColor = vec4(0, 0, 0, 1);
    TexCoord = aTexCoord;
};