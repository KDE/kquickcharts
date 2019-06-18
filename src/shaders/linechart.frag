uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;

uniform highp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

varying highp vec2 uv;


void main()
{
    vec2 point = uv;

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    float polygon = sdf_polygon(point, points, pointCount);

    color = sdf_render(polygon, color, fillColor, 0.001);

    float halfLW = lineWidth / 2.0;
    color = sdf_render(sdf_outline(polygon), color, lineColor, halfLW);

    gl_FragColor = color * opacity;
}
