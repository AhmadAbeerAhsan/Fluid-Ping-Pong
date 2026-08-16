#version 330 core

layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vNormal;

struct PositionalLight
{
    vec4 global_ambient;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform PositionalLight light;
uniform Material material;

out vec3 oVaryingNormal;
out vec3 oVaryingLightDir;
out vec3 oVaryingVertPos;

out PositionalLight oLight;
out Material oMaterial;

void main()
{
    oLight = light;
    oMaterial = material;

    //convert vertex to view space
    vec4 P = view * model * vec4(vVertex, 1.0f);
    //normalize normal
    oVaryingNormal = normalize((view * model * vec4(vNormal, 0.0f)).xyz);
    //calculate view space light vector(from vertex to light)
    vec3 lightPosView = (view * vec4(light.position, 1.0f)).xyz;
    oVaryingLightDir = normalize(lightPosView - P.xyz);

    //view vector is equilvalent to the negative of view space vertex position
    oVaryingVertPos = normalize(-P.xyz);

    gl_Position = projection * view * model * vec4(vVertex, 1);
};