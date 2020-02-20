/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

// This requires "sdf.frag" which is included through SDFShader.

uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;
uniform lowp vec2 bounds;

uniform lowp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

varying lowp vec2 uv;

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
        gl_FragColor = fillColor * opacity;
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

    gl_FragColor = color * opacity;
}
