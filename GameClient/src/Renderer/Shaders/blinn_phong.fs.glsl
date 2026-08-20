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

in vec3 oVaryingNormal;
in vec3 oVaryingLightDir;
in vec3 oVaryingVertPos;

in vec3 oWorldPosition;
in vec3 oWorldNormal;

in vec2 TexCoord;
in vec4 shadow_coord;
in vec3 oColor;

uniform sampler2D ourTexture;   //0
uniform sampler2DShadow shTex;  //1
uniform samplerCube skybox;     //2
uniform sampler2D scene;        //3

uniform bool enableReflection;
uniform bool useColor;
uniform vec3 cameraPosition;
uniform mat4 projection;
uniform vec2 resolution;

uniform bool send_time;
uniform float time;

uniform PositionalLight light;

out vec4 FragColor;

struct HitInfo
{
    ivec2 tc;
    vec3 v_pos;
    float length;
    int steps;
};

void swap(inout float a, inout float b)
{
    float tmp = a;
    a         = b;
    b         = tmp;
}

const float zThickness = 0.5f;
const int rayMaxSteps = 30;
const float shininess = 51.2f;

bool castScreenspaceRay(vec3 v_start, vec3 v_direction, out HitInfo hitinfo, float max_distance)
{
    float ray_length = max_distance;
    if( (v_start.z + v_direction.z * max_distance) > 0)
        ray_length = -v_start.z / v_direction.z;
    
    vec3 v_end = v_start + v_direction * ray_length;

    // Project into clip-space (c_)
    vec4 c_start = projection * vec4(v_start, 1.0);
    vec4 c_end   = projection * vec4(v_end, 1.0);

    // Screen-space endpoints (s_)
    vec2 s_start = c_start.xy / c_start.w;
    vec2 s_end   = c_end.xy / c_end.w;  // in [-1, 1]   clip-space
    s_start      = s_start * 0.5 + vec2(0.5);
    s_end        = s_end * 0.5 + vec2(0.5);  // in [0, 1]   uv - texture coordinates
    s_start *= resolution;
    s_end   *= resolution;  // in [0, resolution]   pixel coordinates

    // prepare the interpolation of screenspace-coordinates along the ray.
    vec2 s_diff = s_end - s_start;
    float num_steps = max(abs(s_diff.x), abs(s_diff.y));
    vec2 s_delta = s_diff / num_steps;

    // prepare the interpolation of viewspace ray-coordinates along the ray.
    // Note the following:
    //   To perform projected linear 3D interpolation in 2D,
    //   h_start and h_end are homogenized based on the projected v_start and v_end homogenious (h_) coordinates (see Q0... in accompanying paper)
    vec4 h_start = vec4(v_start.xyz, 1) / c_start.w;
    vec4 h_end   = vec4(v_end.xyz  , 1) / c_end.w;
    vec4 h_delta = (h_end - h_start) / num_steps;
    
    // start interpolation
    vec2 s_current = s_start;
    vec4 h_current = h_start;

    // apply Stride (to visit every stride-th pixel)
    float rayStride = 10.0f; // to set later
    s_delta *= rayStride;  
    h_delta *= rayStride;

    int stepCount = 0;
    bool foundHit     = false;
    float rayZ_prev = v_start.z;
    int maxSteps  = min(rayMaxSteps, int(num_steps / rayStride));
    
    // for each pixel on ray
    for (; 
        stepCount < maxSteps;
        ++stepCount, 
        s_current += s_delta, 
        h_current += h_delta)
    {
        // TODO:  b) Read scene z-coordinate at the current Pixel.
        //           Note texelFetch(position, ivec2(TC), 0)
        //           and setup the scene Interval   (of width zThickness)
        //           Note that "forward" is going in negative z direction

        ivec2 TC = ivec2(s_current);
        float sceneZMax = texelFetch(scene, TC, 0).a;
        float sceneZMin = sceneZMax - zThickness;

        // TODO:  b) Use the homogenized "h_"-vector to compute the ray-z-value at the end of the current pixel (i.e. +0.5 pixels in ray direction)
        //           That and the previous-ray-z-value form the ray-interval  
        float rayZ_old = rayZ_prev;
        vec4  h_half   = h_current + h_delta * 0.5;
        float rayZ_new = h_half.z / h_half.w;
        rayZ_prev      = rayZ_new;

        float rayZMin = rayZ_old;
        float rayZMax = rayZ_new;
        if (rayZMin > rayZMax)
            swap(rayZMin, rayZMax);

        // TODO:  b)  
        // Check for overlap of
        //          ray-interval     [     ???]
        //      and scene-interval        [???     ]
        // abort the loop if an overlap occures
        // AND set the foundHit flag accordingly

        if (rayZMax >= sceneZMin && rayZMin <= sceneZMax)
        {
            foundHit = true;
            break;
        }
    }

    // collect hit info for later usage
    hitinfo.v_pos  = ( h_current.xyz / h_current.w );
    hitinfo.tc     = ivec2(s_current);
    hitinfo.steps  = stepCount;
    hitinfo.length = length( hitinfo.v_pos - v_start );
    
    //if (foundHit)
    //{
    //    foundHit = all(lessThanEqual(abs(vec2(hitinfo.tc) - resolution * 0.5), resolution * 0.5));
    //}

    return foundHit;
}

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
    vec2 texture_shift = vec2(0.0f, 0.0f);
    if(send_time)
        texture_shift.x += time;

    vec4 material_color;
    if(useColor)
    {
        material_color = vec4(oColor, 1.0f);
    }
    else
    {
        material_color = texture(ourTexture, (TexCoord + texture_shift));
    }

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
    
    vec3 ambient = ((light.global_ambient * material_color) + (light.ambient * material_color)).xyz;
    FragColor = vec4(ambient, -oVaryingVertPos.z);

    vec3 diffuse = light.diffuse.xyz * cosTheta;
    vec3 specular = light.specular.xyz * pow(cosPhi, shininess * 3.0f);

    FragColor += shadow_factor * vec4((diffuse + specular), 0.0f);

    if(!enableReflection)
        return;

    vec3 I = normalize(oWorldPosition - cameraPosition);
    vec3 R = reflect(I, normalize(oWorldNormal));

    vec3 view_reflect_ray_dir = normalize(reflect(-V, N));
    vec3 view_reflect_ray_pos = -oVaryingVertPos + view_reflect_ray_dir;
    vec4 reflected_color;
    HitInfo reflect_hitinfo;
    bool res = castScreenspaceRay(
        view_reflect_ray_pos,
        view_reflect_ray_dir,
        reflect_hitinfo,
        100
    );
    if(res)
    {
        reflected_color = vec4(texelFetch(scene, reflect_hitinfo.tc, 0).rgb, 1.0f);
    }
    else
    {
        reflected_color = texture(skybox, R);
    }


    float cosTheta2 = max(dot(N, V), 0.0);
    float F0 = 0.09;
    float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta2, 5.0);

    FragColor.rgb = mix(FragColor.rgb, reflected_color.rgb, fresnel);
}