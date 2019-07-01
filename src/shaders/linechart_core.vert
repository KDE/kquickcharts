uniform mat4 matrix;
uniform float lineWidth;
uniform float aspect;

in vec4 in_vertex;
in vec2 in_uv;

out vec2 uv;

void main() {
    uv.x = in_uv.x;
    uv.y = 0.5 * lineWidth + (1.0 - 2.0 *  lineWidth) * in_uv.y;
    uv.y = (1.0 + -1.0 * uv.y) * aspect;
    gl_Position = matrix * in_vertex;
}
