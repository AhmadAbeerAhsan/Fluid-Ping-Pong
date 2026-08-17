#version 330 core

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

in vec3 oVaryingNormal;
in vec3 oVaryingLightDir;
in vec3 oVaryingVertPos;

in PositionalLight oLight;
in Material oMaterial;

in vec2 TexCoord;
in vec4 shadow_coord;

uniform sampler2D ourTexture;
uniform sampler2DShadow shTex;

out vec4 FragColor;
    
float lookUp(float ox, float oy)
{
    const float bias = 0.0000001f;

    float t = textureProj(
        shTex,
        shadow_coord + vec4(
            ox * 0.001 * shadow_coord.w,
            oy * 0.001 * shadow_coord.w,
            -bias * shadow_coord.w,
            0.0
        )
    );
    return t;
}

void main()
{
    vec4 texture_color = texture(ourTexture, TexCoord);

    vec3 L = normalize(oVaryingLightDir);
    vec3 N = normalize(oVaryingNormal);
    vec3 V = normalize(oVaryingVertPos);
    vec3 H = normalize(L + V);

    float shadow_factor = 0.0f;
    float swidth = 2.5f;

    vec2 offset = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
    shadow_factor += lookUp(-1.5*swidth + offset.x,  1.5*swidth-offset.y);
    shadow_factor += lookUp(-1.5*swidth + offset.x, -0.5*swidth-offset.y);
    shadow_factor += lookUp( 0.5*swidth + offset.x,  1.5*swidth-offset.y);
    shadow_factor += lookUp( 0.5*swidth + offset.x, -0.5*swidth-offset.y);
    shadow_factor = shadow_factor/4.0f;

    //angle between the view and reflected light
    float cosTheta = max(dot(N,L), 0.0f);
    //angle between the view vector and reflected light
    float cosPhi = max(dot(H,N), 0.0f);

    //float notInShadow = textureProj(shTex, shadow_coord);
    
    vec3 ambient = ((oLight.global_ambient * texture_color) + (oLight.ambient * texture_color)).xyz;
    FragColor = vec4(ambient, 1.0f);

    vec3 diffuse = oLight.diffuse.xyz * cosTheta;
    vec3 specular = oLight.specular.xyz * pow(cosPhi, oMaterial.shininess * 3.0f);

    FragColor += shadow_factor * vec4((diffuse + specular), 1.0f);
}