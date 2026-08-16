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

out vec4 varyingColor;

void main()
{

    //convert vertex to view space
    vec4 P = view * model * vec4(vVertex, 1.0f);
    //normalize normal
    vec3 N = normalize((view * model * vec4(vNormal, 0.0f)).xyz);
    //calculate view space light vector(from vertex to light)
    vec3 lightPosView = (view * vec4(light.position, 1.0f)).xyz;
    vec3 L = normalize(lightPosView - P.xyz);

    //view vector is equilvalent to the negative of view space vertex position
    vec3 V = normalize(-P.xyz);

    //R is reflection of -L with respect to surface normal N
    vec3 R = reflect(-L,N);

    //ambient, diffuse, and specular
    vec3 ambient = ((light.global_ambient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(dot(N,L), 0.0f);
    vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(dot(R,V), 0.0f), material.shininess);

    gl_Position = projection * view * model * vec4(vVertex, 1);
    varyingColor = vec4((ambient + diffuse + specular), 1.0f);
};