/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This requires "sdf.frag" which is included through SDFShader.

// The maximum number of segments we can support for a single pie.
// This is based on OpenGL's MAX_FRAGMENT_UNIFORM_COMPONENTS.
// MAX_FRAGMENT_UNIFORM_COMPONENTS is required to be at least 1024.
// Assuming a segment of size 1, each segment needs
// 2 (size of a vec2) * 2 (number of points) + 4 (size of vec4) + 1 (segment size)
// components. We also need to leave some room for the other uniforms.
#define MAX_SEGMENTS 100

uniform lowp float opacity;
uniform lowp float innerRadius;
uniform lowp float outerRadius;
uniform lowp vec4 backgroundColor;
uniform bool smoothEnds;
uniform lowp float fromAngle;
uniform lowp float toAngle;

uniform lowp vec2 segments[MAX_SEGMENTS];
uniform lowp vec4 colors[MAX_SEGMENTS];
uniform int segmentCount;

#ifdef LEGACY_STAGE_INOUT
varying lowp vec2 uv;
#else
in lowp vec2 uv;
out lowp vec4 out_color;
#endif

const lowp vec2 origin = vec2(0.0, 0.0);
const lowp float lineSmooth = 0.001;

lowp float rounded_segment(lowp float from, lowp float to, lowp float inner, lowp float outer, lowp float rounding)
{
    return sdf_torus_segment(uv, from + rounding, to - rounding, inner + rounding, outer - rounding) - rounding;
}

void main()
{
    lowp vec4 color = vec4(0.0);

    lowp float thickness = (outerRadius - innerRadius) / 2.0;
    lowp float rounding = smoothEnds ? thickness : 0.0;

    // Background first, slightly smaller than the actual pie to avoid antialiasing artifacts.
    lowp float background_rounding = (toAngle - fromAngle) >= 2.0 * pi ? 0.001 : rounding + 0.001;
    lowp float background = rounded_segment(fromAngle, toAngle, innerRadius, outerRadius, background_rounding);
    color = sdf_render(background, color, backgroundColor);

    for (int i = 0; i < segmentCount && i < MAX_SEGMENTS; ++i) {
        lowp vec2 segment = segments[i];

        lowp float segment_sdf = rounded_segment(segment.x, segment.y, innerRadius, outerRadius, rounding);
        color = sdf_render(segment_sdf, color, colors[i]);
    }

#ifdef LEGACY_STAGE_INOUT
    gl_FragColor = color * opacity;
#else
    out_color = color * opacity;
#endif
}
