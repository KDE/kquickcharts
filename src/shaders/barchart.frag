/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

uniform lowp vec2 aspect;
uniform lowp float opacity;

#ifdef LEGACY_STAGE_INOUT
varying lowp vec2 uv;
varying mediump vec4 foregroundColor;
varying mediump float value;
#define out_color gl_FragColor
#else
in lowp vec2 uv;
in mediump vec4 foregroundColor;
in mediump float value;
out lowp vec4 out_color;
#endif

void main()
{
    lowp vec4 color = vec4(0.0);

    lowp float foreground = sdf_rectangle(vec2(uv.x, -aspect.y + uv.y + value), vec2(1.0, value));

    color = sdf_render(foreground, color, foregroundColor);

    out_color = color * opacity;
}
