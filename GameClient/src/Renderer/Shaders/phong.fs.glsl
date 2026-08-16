#version 330 core

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

in vec3 oVaryingNormal;
in vec3 oVaryingLightDir;
in vec3 oVaryingVertPos;

in PositionalLight oLight;
in Material oMaterial;

out vec4 FragColor;
    
void main()
{
    vec3 L = oVaryingLightDir;
    vec3 N = oVaryingNormal;
    vec3 V = oVaryingVertPos;

    //R is reflection of -L with respect to surface normal N
    vec3 R = reflect(-L,N);
    //angle between the view and reflected light
    float cosTheta = max(dot(N,L), 0.0f);
    //angle between the view vector and reflected light
    float cosPhi = max(dot(R,V), 0.0f);

    //ambient, diffuse, and specular
    vec3 ambient = ((oLight.global_ambient * oMaterial.ambient) + (oLight.ambient * oMaterial.ambient)).xyz;
    vec3 diffuse = oLight.diffuse.xyz * oMaterial.diffuse.xyz * cosTheta;
    vec3 specular = oLight.specular.xyz * oMaterial.specular.xyz * pow(cosPhi, oMaterial.shininess);

    FragColor = vec4((ambient + diffuse + specular), 1.0f);
}