#version 410 core
#extension GL_ARB_gpu_shader_fp64 : require

out vec4 FragColor;

uniform dvec2 u_resolution;
uniform double zoom;
uniform int limit;
uniform double offsetX;
uniform double offsetY;

const float PI = 3.141592653589793;

// Smooth color palette
dvec3 get_color(float mu)
{
    float t = mu * 0.02;
    vec3 col = 0.5 + 0.5 * cos(2.0 * PI * (t + vec3(0.0, 0.33, 0.67)));
    return dvec3(col);
}

void main()
{
    // Normalized screen coordinates
    dvec2 uv = (dvec2(gl_FragCoord.xy) / u_resolution - 0.5) * 2.0;

    // Aspect ratio
    uv.x *= 1.77777777778;

    // Complex plane
    double x = uv.x * zoom + offsetX;
    double y = uv.y * zoom + offsetY;

    // Cardioid optimization (skip interior)
    double q = (x - 0.25)*(x - 0.25) + y*y;
    if (q * (q + (x - 0.25)) < 0.25 * y*y)
    {
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }

    if ((x+1.0)*(x+1.0) + y*y < 0.0625)
    {
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }

    double zr = 0.0;
    double zi = 0.0;

    double zr2;
    double zi2;

    int it;

    for (it = 0; it < limit; it++)
    {
        zr2 = zr*zr;
        zi2 = zi*zi;

        if (zr2 + zi2 > 4.0)
            break;

        zi = 2.0*zr*zi + y;
        zr = zr2 - zi2 + x;
    }

    if (it == limit)
    {
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }

    // Smooth iteration
    float mag = sqrt(float(zr2 + zi2));
    float mu = it + 1.0 - log(log(mag)) / log(2.0);

    FragColor = vec4(get_color(mu),1.0);
}