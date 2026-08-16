#version 330 core

layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 oColor;

void main()
{
    gl_Position = projection * view * model * vec4(vVertex, 1);
    oColor = vec4(vColor, 1);
};