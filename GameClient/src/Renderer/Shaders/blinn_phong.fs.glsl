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

in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;
    
void main()
{

    vec4 texture_color = texture(ourTexture, TexCoord);

    vec3 L = normalize(oVaryingLightDir);
    vec3 N = normalize(oVaryingNormal);
    vec3 V = normalize(oVaryingVertPos);
    vec3 H = normalize(L + V);

    //angle between the view and reflected light
    float cosTheta = max(dot(N,L), 0.0f);
    //angle between the view vector and reflected light
    float cosPhi = max(dot(H,N), 0.0f);

    //ambient, diffuse, and specular
    vec3 ambient = ((oLight.global_ambient * texture_color) + (oLight.ambient * texture_color)).xyz;
    vec3 diffuse = oLight.diffuse.xyz * cosTheta;
    vec3 specular = oLight.specular.xyz * pow(cosPhi, oMaterial.shininess * 3.0f);

    FragColor = vec4((ambient + diffuse + specular), 1.0f);
}