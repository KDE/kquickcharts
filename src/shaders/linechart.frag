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


void main()
{
    lowp vec2 point = uv;

    lowp vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    // bounds.y contains the line segment's maximum value. If we are a bit above
    // that, we will never render anything, so just discard the pixel.
    if (point.y > bounds.y + 0.01) {
        discard;
    }

    // bounds.x contains the line segment's minimum value. If we are a bit below
    // that, we know we will always be inside the polygon described by points.
    // So just return a pixel with fillColor.
    if (point.y < bounds.x - 0.01) {
#ifdef LEGACY_STAGE_INOUT
        gl_FragColor = fillColor * opacity;
#else
        out_color = fillColor * opacity;
#endif
        return;
    }

#if !defined(GL_ES) || !defined(VALIDATING)
// See sdf.frag line 98
    lowp float polygon = sdf_polygon(point, points, pointCount);
#else
    lowp float polygon = 0.0;
#endif

    color = sdf_render(polygon, color, fillColor, 0.001);

    if (lineWidth > 0.0) {
        color = sdf_render(sdf_annular(sdf_outline(polygon), lineWidth), color, lineColor, 0.0002);
    }

#ifdef LEGACY_STAGE_INOUT
    gl_FragColor = color * opacity;
#else
    out_color = color * opacity;
#endif
}
