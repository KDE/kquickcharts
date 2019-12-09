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

uniform mat4 matrix;
uniform float lineWidth;
uniform float aspect;

in vec4 in_vertex;
in vec2 in_uv;

out vec2 uv;

void main() {
    uv.x = in_uv.x;
    uv.y = in_uv.y;
    uv.y = (1.0 + -1.0 * uv.y) * aspect;
    gl_Position = matrix * in_vertex;
}
