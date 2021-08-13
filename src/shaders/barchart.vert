/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

uniform highp mat4 matrix;
uniform lowp vec2 aspect;

#ifdef LEGACY_STAGE_INOUT
#define in attribute
#define out varying
#endif

in highp vec4 in_vertex;
in mediump vec2 in_uv;
in mediump vec4 in_color;
in mediump float in_value;

out mediump vec2 uv;
out mediump vec4 foregroundColor;
out mediump float value;

void main() {
    uv = (-1.0 + 2.0 * in_uv) * aspect;
    value = in_value;
    foregroundColor = in_color;
    gl_Position = matrix * in_vertex;
}
