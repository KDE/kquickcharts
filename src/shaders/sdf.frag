#version 120

float sdf_circle(in vec2 point, in float radius)
{
    return length(point) - radius;
}

float sdf_triangle(in vec2 point, in vec2 p0, in vec2 p1, in vec2 p2)
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

float sdf_annular(in float sdf, in float thickness)
{
    return abs(sdf) - thickness;
}

float sdf_union(in float sdf1, in float sdf2)
{
    return min(sdf1, sdf2);
}

float sdf_subtract(in float sdf1, in float sdf2)
{
    return max(-sdf1, sdf2);
}

float sdf_intersect(in float sdf1, in float sdf2)
{
    return max(sdf1, sdf2);
}

float sdf_round(in float sdf, in float amount)
{
    return sdf - amount;
}

vec4 sdf_render(in float sdf, in vec4 sourceColor, in vec4 sdfColor, in float smoothing)
{
    float g = fwidth(sdf);
    return mix(sourceColor, sdfColor, sdfColor.a * (1.0 - smoothstep(smoothing - g, smoothing + g, sdf)));
}
