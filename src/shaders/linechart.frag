/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 * SPDX-FileCopyrightText: 2022 Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#version 440

#extension GL_GOOGLE_include_directive: enable
#include "sdf.glsl"

layout(std140, binding = 0) uniform buf {
    highp mat4 matrix;
    highp float opacity; // inherited opacity of this item - offset 64
    highp float lineWidth; // offset 68
    highp float aspect; // offset 72
    highp float smoothing; // offset 76
} ubuf; // size 80

#define MAXIMUM_POINT_COUNT 14

layout (location = 0) in highp vec2 uv;
layout (location = 1) in highp vec4 pointTuples[MAXIMUM_POINT_COUNT / 2];
layout (location = 19) in highp float pointCount;
layout (location = 20) in highp vec2 bounds;
layout (location = 21) in lowp vec4 lineColor;
layout (location = 22) in lowp vec4 fillColor;
layout (location = 0) out lowp vec4 out_color;

// ES2 does not support array function arguments. So instead we need to
// reference the uniform array directly. So this copies the implementation of
// sdf_polygon from sdf.glsl, changing it to refer to the points array directly.
// For simplicity, we use the same function also for other APIs.
highp float sdf_polygon(in highp vec2 point, in int count)
{
    mediump vec2 points[MAXIMUM_POINT_COUNT];
    points[0] = pointTuples[0].xy;
    points[1] = pointTuples[0].zw;
    points[2] = pointTuples[1].xy;
    points[3] = pointTuples[1].zw;
    points[4] = pointTuples[2].xy;
    points[5] = pointTuples[2].zw;
    points[6] = pointTuples[3].xy;
    points[7] = pointTuples[3].zw;
    points[8] = pointTuples[4].xy;
    points[9] = pointTuples[4].zw;
    points[10] = pointTuples[5].xy;
    points[11] = pointTuples[5].zw;
    points[12] = pointTuples[6].xy;
    points[13] = pointTuples[6].zw;

    highp float d = dot(point - points[0], point - points[0]);
    highp float s = 1.0;
    for (int i = 0, j = count - 1; i < count && i < MAXIMUM_POINT_COUNT; j = i, i++)
    {
        highp vec2 e = points[j] - points[i];
        highp vec2 w = point - points[i];
        highp float h = clamp( dot(w, e) / dot(e, e), 0.0, 1.0 );
        highp vec2 b = w - e * h;
        d = min(d, dot(b, b));

        bvec3 c = bvec3(point.y >= points[i].y, point.y < points[j].y, e.x * w.y > e.y * w.x);
        if(all(c) || all(not(c))) s *= -1.0;
    }
    return s * sqrt(d);
}

void main()
{
    highp vec2 point = uv;

    lowp vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    highp float bounds_range = max(0.01, ubuf.lineWidth);

    // bounds.y contains the line segment's maximum value. If we are a bit above
    // that, we will never render anything, so just discard the pixel.
    if (point.y > bounds.y + bounds_range) {
        out_color = vec4(0.0);
        return;
    }

    // bounds.x contains the line segment's minimum value. If we are a bit below
    // that, we know we will always be inside the polygon described by points.
    // So just return a pixel with fillColor.
    if (point.y < bounds.x - bounds_range) {
        out_color = fillColor * ubuf.opacity;
        return;
    }

    highp float polygon = sdf_polygon(point, int(pointCount));

    color = sdf_render(polygon, color, fillColor);

    if (ubuf.lineWidth > 0.0) {
        color = mix(color, lineColor, 1.0 - smoothstep(-ubuf.smoothing, ubuf.smoothing, sdf_annular(polygon, ubuf.lineWidth)));
    }

    out_color = color * ubuf.opacity;
}
