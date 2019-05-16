uniform highp mat4 matrix;

attribute highp vec4 in_vertex;
attribute mediump vec2 in_uv;

varying mediump vec2 uv;

void main() {
    uv = in_uv;
    gl_Position = matrix * in_vertex;
}
