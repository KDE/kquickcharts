
// The maximum number of segments we can support for a single pie.
// This is based on OpenGL's MAX_FRAGMENT_UNIFORM_COMPONENTS.
// MAX_FRAGMENT_UNIFORM_COMPONENTS is required to be at least 1024.
// Assuming a segment of size 1, each segment needs
// 2 (size of a vec2) * 2 (number of points) + 4 (size of vec4) + 1 (segment size)
// components. We also need to leave some room for the other uniforms.
#define MAX_SEGMENTS 100

// Note: See sdf.frag for the distance field related functions. It gets "included" for each shader.

uniform lowp float opacity;
uniform mediump float innerDimension;
uniform mediump vec2 aspect;
uniform lowp vec4 backgroundColor;

uniform mediump vec2 triangles[MAX_SEGMENTS * 2];
uniform mediump vec4 colors[MAX_SEGMENTS];
uniform int segments[MAX_SEGMENTS];
uniform int segmentCount;

varying mediump vec2 uv;

const vec2 origin = vec2(0.0, 0.0);
const float lineSmooth = 0.001;

void main()
{
    vec2 point = uv * (1.0 + lineSmooth * 2.0) * aspect;

    float thickness = (1.0 - innerDimension) / 2.0;
    float donut = sdf_annular(sdf_circle(point, innerDimension + thickness), thickness);

    vec4 color = vec4(0.0);
    float totalSegments = 99999.0;
    int index = 0;

    for (int i = 0; i < segmentCount && i < MAX_SEGMENTS; ++i) {
        float segment = 99999.0;
        for(int j = 0; j < segments[i] && j < MAX_SEGMENTS; j++) {
            segment = sdf_union(segment, sdf_round(sdf_triangle(point, origin, triangles[index++], triangles[index++]), lineSmooth));
        }
        totalSegments = sdf_union(totalSegments, segment);
        color = sdf_render(sdf_intersect(donut, segment), color, colors[i], lineSmooth);
    }

    // Finally, render an end segment with the background color.
    float segment = sdf_subtract(totalSegments, sdf_round(donut, lineSmooth));
    color = sdf_render(segment, color, backgroundColor, lineSmooth);

    gl_FragColor = color * opacity;
}
