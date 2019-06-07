
#define VERSION 120
// The maximum number of points we can support for a single pie.
// This is based on OpenGL's MAX_FRAGMENT_UNIFORM_COMPONENTS.
// MAX_FRAGMENT_UNIFORM_COMPONENTS is required to be at least 1024.
// Each triangle needs 2 (size of a vec2) * 2 (number of points) + 4 (size of vec4)
// components. We also need to leave some room for the other uniforms.
#define MAX_TRIANGLE_POINTS 200

uniform lowp float opacity;
uniform mediump float innerDimension;
uniform mediump vec2 aspect;

uniform mediump vec2 triangles[MAX_TRIANGLE_POINTS];
uniform mediump vec4 colors[MAX_TRIANGLE_POINTS / 2];
uniform int triangleCount;

varying mediump vec2 uv;

const vec2 origin = vec2(0.0, 0.0);
const float lineSmooth = 0.01;

float donut(in vec2 point, in float radius, in float thickness)
{
    return abs(length(point) - radius) - thickness;
}

float triangle(in vec2 point, in vec2 p0, in vec2 p1, in vec2 p2)
{
    vec2 e0 = p1 - p0;
    vec2 e1 = p2 - p1;
    vec2 e2 = p0 - p2;

    vec2 v0 = point - p0;
    vec2 v1 = point - p1;
    vec2 v2 = point - p2;

    vec2 pq0 = v0 - e0 * clamp( dot(v0, e0) / dot(e0, e0), 0.0, 1.0 );
    vec2 pq1 = v1 - e1 * clamp( dot(v1, e1) / dot(e1, e1), 0.0, 1.0 );
    vec2 pq2 = v2 - e2 * clamp( dot(v2, e2) / dot(e2, e2), 0.0, 1.0 );

    float s = sign( e0.x*e2.y - e0.y*e2.x );
    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));

    return -sqrt(d.x)*sign(d.y);
}

void main()
{
    vec2 point = uv * (1.0 + lineSmooth * 2.0) * aspect;

    float thickness = (1.0 - innerDimension) / 2.0;
    float d = donut(point, innerDimension + thickness, thickness);

    vec4 col = vec4(0.0);
    for(int i = 0; i < triangleCount; i+=2)
    {
        float dist = max(d, triangle(point, origin, triangles[i], triangles[i+1]) - lineSmooth);
        float g = fwidth(dist);
        col = mix(col, colors[i / 2], colors[i / 2].a * (1.0 - smoothstep(lineSmooth - g, lineSmooth + g, dist)));
    }

    gl_FragColor = col * opacity;
}
