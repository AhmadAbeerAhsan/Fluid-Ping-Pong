#version 330 core
layout (location = 0) in vec3 vVertex;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = vVertex;
    gl_Position = projection * view * model * vec4(vVertex, 1.0);
}  