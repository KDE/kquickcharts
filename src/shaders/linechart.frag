//include "sdf.frag"

uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;

uniform lowp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

varying lowp vec2 uv;

void main()
{
    lowp vec2 point = uv;

    lowp vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    lowp float polygon = sdf_polygon(point, points, pointCount);

    color = sdf_render(polygon, color, fillColor, 0.001);

    lowp float halfLW = lineWidth / 2.0;
    color = sdf_render(sdf_outline(polygon), color, lineColor, halfLW);

    gl_FragColor = color * opacity;
}
