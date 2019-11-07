/*
 * This file is part of KQuickCharts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// This requires "sdf.frag" which is included through SDFShader.

uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;

uniform lowp vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

varying lowp vec2 uv;

void main()
{
    lowp vec2 point = uv;

    lowp vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

#if !defined(GL_ES) || !defined(VALIDATING)
// See sdf.frag line 98
    lowp float polygon = sdf_polygon(point, points, pointCount);
#else
    lowp float polygon = 0.0;
#endif

    color = sdf_render(polygon, color, fillColor, 0.001);

    color = sdf_render(sdf_annular(sdf_outline(polygon), lineWidth), color, lineColor, 0.0004);

    gl_FragColor = color * opacity;
}
