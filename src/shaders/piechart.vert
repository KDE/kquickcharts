uniform highp mat4 matrix;

attribute highp vec4 in_vertex;
attribute mediump vec2 in_uv;

varying mediump vec2 uv;

void main() {
    uv = -1.0 + 2.0 * in_uv;
    gl_Position = matrix * in_vertex;
}
