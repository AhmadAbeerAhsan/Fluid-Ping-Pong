#version 330 core
out vec4 FragColor;

in vec3 ViewNormal;
in vec3 WorldNormal;
in vec3 ViewPosition;
in vec3 WorldPosition;
in vec2 TexCoord;

uniform vec3 cameraPosition;
uniform mat4 projection;
uniform vec2 resolution;

uniform samplerCube skybox;
uniform sampler2D scene;

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
    float rayStride = 5.0f; // to set later
    s_delta *= rayStride;  
    h_delta *= rayStride;

    int stepCount = 0;
    bool foundHit     = false;
    float rayZ_prev = v_start.z;
    int rayMaxSteps = 100; 
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
        float zThickness = 2.0f;
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

void main()
{    
    vec3 I = normalize(WorldPosition - cameraPosition);
    vec3 R = reflect(I, normalize(WorldNormal));

    //raytrace
    vec3 incident = normalize(ViewPosition);
    vec3 n = normalize(ViewNormal);
    vec3 reflect_ray_dir = normalize(reflect(incident, n));
    vec3 reflect_ray_pos = ViewPosition + reflect_ray_dir;

    HitInfo refract_hitinfo;

    FragColor = texture(skybox, R);
    bool res = castScreenspaceRay(
        reflect_ray_pos,
        reflect_ray_dir,
        refract_hitinfo,
        100
    );
    if(res)
    {
        FragColor = vec4(texelFetch(scene, refract_hitinfo.tc, 0).rgb, 1.0f);
    }
}