#version 330 core
layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 ViewNormal;
out vec3 WorldNormal;
out vec3 ViewPosition;
out vec3 WorldPosition;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
    WorldNormal = (model * vec4(vNormal, 0.0f)).xyz;
    ViewNormal = (view * vec4(WorldNormal, 0.0f)).xyz;
    WorldPosition = (model * vec4(vVertex, 1.0f)).xyz;
    ViewPosition = (view * vec4(WorldPosition, 1.0f)).xyz;

    TexCoord = aTexCoord;

    gl_Position = projection * vec4(ViewPosition, 1.0f);
}  