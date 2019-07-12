//include "sdf.frag"

uniform float opacity;
uniform vec4 lineColor;
uniform vec4 fillColor;
uniform float lineWidth;

uniform vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

in vec2 uv;

out vec4 out_color;

void main()
{
    vec2 point = uv;

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    float polygon = sdf_polygon(point, points, pointCount);

    color = sdf_render(polygon, color, fillColor, 0.001);

    color = sdf_render(sdf_annular(sdf_outline(polygon), lineWidth), color, lineColor, 0.0004);

    out_color = color * opacity;
}
