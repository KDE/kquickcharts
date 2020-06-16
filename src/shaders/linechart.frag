/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;
uniform lowp vec2 bounds;

uniform lowp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

#ifdef LEGACY_STAGE_INOUT
varying lowp vec2 uv;
#else
in lowp vec2 uv;
out lowp vec4 out_color;
#endif

#ifdef API_ES2
// ES2 does not support array function arguments. So instead we need to
// reference the uniform array directly. So this copies the implementation of
// sdf_polygon from sdf.glsl, changing it to refer to the points array directly.
lowp float sdf_polygon(in lowp vec2 point, in lowp int count)
{
    lowp float d = dot(point - points[0], point - points[0]);
    lowp float s = 1.0;
    for (int i = 0, j = count - 1; i < count && i < SDF_POLYGON_MAX_POINT_COUNT; j = i, i++)
    {
        lowp vec2 e = points[j] - points[i];
        lowp vec2 w = point - points[i];
        lowp float h = clamp( dot(w, e) / dot(e, e), 0.0, 1.0 );
        lowp vec2 b = w - e * h;
        d = min(d, dot(b, b));

        bvec3 c = bvec3(point.y >= points[i].y, point.y < points[j].y, e.x * w.y > e.y * w.x);
        if(all(c) || all(not(c))) s *= -1.0;
    }
    return s * sqrt(d);
}
#endif

void main()
{
    lowp vec2 point = uv;

    lowp vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    lowp float bounds_range = max(0.01, lineWidth);

    // bounds.y contains the line segment's maximum value. If we are a bit above
    // that, we will never render anything, so just discard the pixel.
    if (point.y > bounds.y + bounds_range) {
        discard;
    }

    // bounds.x contains the line segment's minimum value. If we are a bit below
    // that, we know we will always be inside the polygon described by points.
    // So just return a pixel with fillColor.
    if (point.y < bounds.x - bounds_range) {
#ifdef LEGACY_STAGE_INOUT
        gl_FragColor = fillColor * opacity;
#else
        out_color = fillColor * opacity;
#endif
        return;
    }

#ifdef API_ES2
    lowp float polygon = sdf_polygon(point, pointCount);
#else
    lowp float polygon = sdf_polygon(point, points, pointCount);
#endif

    color = sdf_render(polygon, color, fillColor);

    if (lineWidth > 0.0) {
        color = mix(color, lineColor, 1.0 - smoothstep(-0.001, 0.001, sdf_annular(polygon, lineWidth)));
    }

#ifdef LEGACY_STAGE_INOUT
    gl_FragColor = color * opacity;
#else
    out_color = color * opacity;
#endif
}
