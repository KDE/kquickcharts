
#define VERSION 120
#define MAX_TRIANGLE_POINTS 32

uniform lowp float opacity;
uniform mediump float innerDimension;
uniform mediump vec2 aspect;
//uniform lowp float edgeBlur;

uniform mediump vec2 triangles[MAX_TRIANGLE_POINTS];
uniform mediump vec4 colors[MAX_TRIANGLE_POINTS];
uniform int triangleCount;

varying mediump vec2 uv;

float donut(in vec2 point, in float radius, in float thickness)
{
    return abs(length(point) - radius) - thickness;
}

float combineIntersect(in float d0, in float d1)
{
    return max(d0, d1);
}

float triangle(in vec2 point, in vec2 p0, in vec2 p1, in vec2 p2)
{
    vec2 e0 = p1 - p0;
    vec2 e1 = p2 - p1;
    vec2 e2 = p0 - p2;

    vec2 v0 = point - p0;
    vec2 v1 = point - p1;
    vec2 v2 = point - p2;

    vec2 pq0 = v0 - e0 * clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
    vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
    vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );

    float s = sign( e0.x*e2.y - e0.y*e2.x );
    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));

    return -sqrt(d.x)*sign(d.y);
}

float renderPart(in float donut, in vec2 uv, in vec2 p0, in vec2 p1, in vec2 p2)
{
    float p = 300.0 * combineIntersect(triangle(uv, p0, p1, p2), donut);
    return max(0.0, 1.0 - max(0.0, p)); // 0.0, clamp(-sign(p), 0.0, 1.0);
}

void main()
{
    vec2 point = (-1.0 + 2.0 * uv) * aspect;

    float thickness = (1.0 - innerDimension) / 2.0;
    float d = donut(point, innerDimension + thickness, thickness);

    vec4 col = vec4(0.0);
    for(int i = 0; i < triangleCount - 1; i++)
    {
        col += colors[i] * renderPart(d, point, vec2(0.0, 0.0), triangles[i], triangles[i + 1]);
    }

    gl_FragColor = col * opacity;
}
