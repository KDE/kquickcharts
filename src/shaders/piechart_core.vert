uniform mat4 matrix;
uniform vec2 aspect;

in vec4 in_vertex;
in vec2 in_uv;

out vec2 uv;

void main() {
    uv = (-1.0 + 2.0 * in_uv) * aspect;
    gl_Position = matrix * in_vertex;
}
