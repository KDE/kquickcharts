/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 * SPDX-FileCopyrightText: 2022 Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>
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
// attributes, we have to manually declare a number of attributes. We use
// array of vec4 point tuples instead of vec2 to not cross the OpenGL limits
// like e.g. GL_MAX_VERTEX_ATTRIBS for some drivers.
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
out mediump vec4 pointTuples[MAXIMUM_POINT_COUNT / 2];
out highp float pointCount;
out mediump vec2 bounds;
out mediump vec4 lineColor;
out mediump vec4 fillColor;

void main() {
    uv = in_uv;
    uv.y = (1.0 + -1.0 * uv.y) * aspect;

    pointTuples[0] = in_points_0;
    pointTuples[1] = in_points_1;
    pointTuples[2] = in_points_2;
    pointTuples[3] = in_points_3;
    pointTuples[4] = in_points_4;
    pointTuples[5] = in_points_5;
    pointTuples[6] = in_points_6;
    pointTuples[7] = in_points_7;
    pointTuples[8] = in_points_8;

    pointCount = in_count;
    bounds = in_bounds;

    lineColor = in_lineColor;
    fillColor = in_fillColor;

    gl_Position = matrix * in_vertex;
}
