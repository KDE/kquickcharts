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
