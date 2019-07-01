uniform highp mat4 matrix;

in vec4 in_vertex;
in vec2 in_uv;

out vec2 uv;

void main() {
    uv = -1.0 + 2.0 * in_uv;
    gl_Position = matrix * in_vertex;
}
