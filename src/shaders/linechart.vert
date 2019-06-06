uniform highp mat4 matrix;
uniform lowp float lineWidth;
uniform lowp float aspect;

attribute highp vec4 in_vertex;
attribute mediump vec2 in_uv;

varying mediump vec2 uv;

void main() {
    uv.x = lineWidth + (1.0 - 2.0 * lineWidth) * in_uv.x;
    uv.y = 0.5 * lineWidth + (1.0 - 2.0 *  lineWidth) * in_uv.y;
    uv.y = (1.0 + -1.0 * uv.y) * aspect;
    gl_Position = matrix * in_vertex;
}
