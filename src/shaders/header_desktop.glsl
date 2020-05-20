/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

// This file contains common directives needed for the shaders to work.
// It is included as the very first bit in the shader.
// Important: If a specific GLSL version is needed, it should be set in this
// file.

// This file is intended for desktop OpenGL version 2.1 or greater.

#version 120

#ifdef VALIDATING
    #define lowp
    #define mediump
    #define highp
#endif

#define API_DESKTOP
#define LEGACY_STAGE_INOUT
