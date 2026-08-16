#version 330 core

layout (location = 0) in vec3 vVertex;

uniform mat4 model;
uniform mat4 shadow_view;
uniform mat4 shadow_proj;

void main()
{
    gl_Position = shadow_proj * shadow_view * model * vec4(vVertex, 1.0f);
};