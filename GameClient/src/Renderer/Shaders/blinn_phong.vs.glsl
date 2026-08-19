#version 330 core

layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 aTexCoord;

struct PositionalLight
{
    vec4 global_ambient;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
    mat4 proj_view;
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

out vec3 oWorldPosition;
out vec3 oWorldNormal;

out PositionalLight oLight;
out Material oMaterial;

out vec2 TexCoord;
out vec4 shadow_coord;

void main()
{
    oLight = light;
    oMaterial = material;
    TexCoord = aTexCoord;

    oWorldPosition = (model * vec4(vVertex, 1.0f)).xyz;
    oWorldNormal = (model * vec4(vNormal, 0.0f)).xyz;
    
    //convert vertex to view space
    vec4 P = view * vec4(oWorldPosition, 1.0f);
    shadow_coord = light.proj_view * model * vec4(vVertex, 1.0f);
    //normalize normal
    oVaryingNormal = (view * vec4(oWorldNormal, 0.0f)).xyz;
    //calculate view space light vector(from vertex to light)
    vec3 lightPosView = (view * vec4(light.position, 1.0f)).xyz;
    oVaryingLightDir = lightPosView - P.xyz;

    //view vector is equilvalent to the negative of view space vertex position
    oVaryingVertPos = -P.xyz;

    gl_Position = projection * P;
};