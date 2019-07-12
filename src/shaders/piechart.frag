//include "sdf.frag"

// The maximum number of segments we can support for a single pie.
// This is based on OpenGL's MAX_FRAGMENT_UNIFORM_COMPONENTS.
// MAX_FRAGMENT_UNIFORM_COMPONENTS is required to be at least 1024.
// Assuming a segment of size 1, each segment needs
// 2 (size of a vec2) * 2 (number of points) + 4 (size of vec4) + 1 (segment size)
// components. We also need to leave some room for the other uniforms.
#define MAX_SEGMENTS 100

uniform lowp float opacity;
uniform lowp float innerRadius;
uniform lowp float outerRadius;
uniform lowp vec4 backgroundColor;

uniform lowp vec2 triangles[MAX_SEGMENTS * 2];
uniform lowp vec4 colors[MAX_SEGMENTS];
uniform int segments[MAX_SEGMENTS];
uniform int segmentCount;

varying lowp vec2 uv;

const lowp vec2 origin = vec2(0.0, 0.0);
const lowp float lineSmooth = 0.001;

void main()
{
    lowp vec2 point = uv * (1.0 + lineSmooth * 2.0);

    lowp float thickness = (outerRadius - innerRadius) / 2.0;
    lowp float donut = sdf_annular(sdf_circle(point, innerRadius + thickness), thickness);

    lowp vec4 color = vec4(0.0);
    lowp float totalSegments = sdf_null;
    int index = 0;

    for (int i = 0; i < segmentCount && i < MAX_SEGMENTS; ++i) {
        lowp float segment = sdf_null;
        for(int j = 0; j < segments[i] && j < MAX_SEGMENTS; j++) {
            segment = sdf_union(segment, sdf_round(sdf_triangle(point, origin, triangles[index++], triangles[index++]), lineSmooth));
        }
        totalSegments = sdf_union(totalSegments, segment);
        color = sdf_render(sdf_intersect(donut, segment), color, colors[i], lineSmooth);
    }

    // Finally, render an end segment with the background color.
    lowp float segment = sdf_subtract(sdf_round(donut, lineSmooth), totalSegments);
    color = sdf_render(segment, color, backgroundColor, lineSmooth);

    gl_FragColor = color * opacity;
}
