/*
 * This file is part of Quick Charts.
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

// This requires "sdf_core.frag" which is included through SDFShader.

uniform float opacity;
uniform vec4 lineColor;
uniform vec4 fillColor;
uniform float lineWidth;

uniform vec2 points[SDF_POLYGON_MAX_POINT_COUNT];
uniform int pointCount;

in vec2 uv;

out vec4 out_color;

void main()
{
    vec2 point = uv;

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    float polygon = sdf_polygon(point, points, pointCount);

    color = sdf_render(polygon, color, fillColor, 0.001);

    color = sdf_render(sdf_annular(sdf_outline(polygon), lineWidth), color, lineColor, 0.0004);

    out_color = color * opacity;
}
