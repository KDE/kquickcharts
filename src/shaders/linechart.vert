/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#line 8

uniform highp mat4 matrix;
uniform lowp float lineWidth;
uniform lowp float aspect;

#define MAXIMUM_POINT_COUNT 18

#ifdef LEGACY_STAGE_INOUT
#define in attribute
#define out varying
#endif

in highp vec4 in_vertex;
in mediump vec2 in_uv;

in mediump vec4 in_lineColor;
in mediump vec4 in_fillColor;
in mediump vec2 in_bounds;

in highp float in_count;

// Input points. Since OpenGL 2.1/GLSL 1.10 does not support array vertex
// attributes, we have to manually declare a number of attributes. In addition,
// while we use vec2 as point data format, we can store either a vec2 or a vec4
// as a vertex attribute. Therefore, we define these as vec4 and manually unpack
// to make use of as much storage as possible.
in mediump vec4 in_points_0;
in mediump vec4 in_points_1;
in mediump vec4 in_points_2;
in mediump vec4 in_points_3;
in mediump vec4 in_points_4;
in mediump vec4 in_points_5;
in mediump vec4 in_points_6;
in mediump vec4 in_points_7;
in mediump vec4 in_points_8;

out mediump vec2 uv;
out mediump vec2 points[MAXIMUM_POINT_COUNT];
out highp float pointCount;
out mediump vec2 bounds;
out mediump vec4 lineColor;
out mediump vec4 fillColor;

void main() {
    uv = in_uv;
    uv.y = (1.0 + -1.0 * uv.y) * aspect;

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

    gl_Position = matrix * in_vertex;
}
