/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

uniform highp mat4 matrix;
uniform lowp vec2 aspect;

#ifdef LEGACY_STAGE_INOUT
attribute highp vec4 in_vertex;
attribute mediump vec2 in_uv;
varying mediump vec2 uv;
#else
in highp vec4 in_vertex;
in mediump vec2 in_uv;
out mediump vec2 uv;
#endif

void main() {
    uv = (-1.0 + 2.0 * in_uv) * aspect;
    uv.y *= -1.0;
    gl_Position = matrix * in_vertex;
}
