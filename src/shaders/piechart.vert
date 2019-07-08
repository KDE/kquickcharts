uniform highp mat4 matrix;
uniform lowp vec2 aspect;

attribute highp vec4 in_vertex;
attribute mediump vec2 in_uv;

varying mediump vec2 uv;

void main() {
    uv = (-1.0 + 2.0 * in_uv) * aspect;
    gl_Position = matrix * in_vertex;
}
