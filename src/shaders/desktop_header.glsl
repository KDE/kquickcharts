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

// This file contains common directives needed for the shaders to work.
// It is included as the very first bit in the shader.
// Important: If a specific GLSL version is needed, it should be set in this
// file.

// This file is intended for desktop OpenGL version 2.1 or greater.

#version 120

#ifndef lowp
    #define lowp
#endif

#ifndef mediump
    #define mediump
#endif

#ifndef highp
    #define highp mediump
#endif
