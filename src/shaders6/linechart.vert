/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#version 440
#line 8

layout(std140, binding = 0) uniform buf {
    highp mat4 matrix;
    lowp float lineWidth;
    lowp float aspect;
    lowp float opacity;
    lowp float smoothing;
} ubuf;

#define MAXIMUM_POINT_COUNT 18

layout (location = 0) in highp vec4 in_vertex;
layout (location = 1) in mediump vec2 in_uv;

layout (location = 2) in mediump vec4 in_lineColor;
layout (location = 3) in mediump vec4 in_fillColor;
layout (location = 4) in mediump vec2 in_bounds;

layout (location = 5) in highp float in_count;

// Input points. Since OpenGL 2.1/GLSL 1.10 does not support array vertex
// attributes, we have to manually declare a number of attributes. In addition,
// while we use vec2 as point data format, we can store either a vec2 or a vec4
// as a vertex attribute. Therefore, we define these as vec4 and manually unpack
// to make use of as much storage as possible.
layout (location = 6) in mediump vec4 in_points_0;
layout (location = 8) in mediump vec4 in_points_1;
layout (location = 9) in mediump vec4 in_points_2;
layout (location = 10) in mediump vec4 in_points_3;
layout (location = 11) in mediump vec4 in_points_4;
layout (location = 12) in mediump vec4 in_points_5;
layout (location = 13) in mediump vec4 in_points_6;
layout (location = 14) in mediump vec4 in_points_7;
layout (location = 15) in mediump vec4 in_points_8;

layout (location = 0) out mediump vec2 uv;
layout (location = 1) out mediump vec2 points[MAXIMUM_POINT_COUNT];
layout (location = 19) out highp float pointCount;
layout (location = 20) out mediump vec2 bounds;
layout (location = 21) out mediump vec4 lineColor;
layout (location = 22) out mediump vec4 fillColor;

void main() {
    uv = in_uv;
    uv.y = (1.0 + -1.0 * uv.y) * ubuf.aspect;

    points[0] = in_points_0.xy;
    points[1] = in_points_0.zw;
    points[2] = in_points_1.xy;
    points[3] = in_points_1.zw;
    points[4] = in_points_2.xy;
    points[5] = in_points_2.zw;
    points[6] = in_points_3.xy;
    points[7] = in_points_3.zw;
    points[8] = in_points_4.xy;
    points[9] = in_points_4.zw;
    points[10] = in_points_5.xy;
    points[11] = in_points_5.zw;
    points[12] = in_points_6.xy;
    points[13] = in_points_6.zw;
    points[14] = in_points_7.xy;
    points[15] = in_points_7.zw;
    points[16] = in_points_8.xy;
    points[17] = in_points_8.zw;

    pointCount = in_count;
    bounds = in_bounds;

    lineColor = in_lineColor;
    fillColor = in_fillColor;

    gl_Position = ubuf.matrix * in_vertex;
}
