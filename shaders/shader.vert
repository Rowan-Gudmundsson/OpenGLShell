#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_uv;

smooth out vec2 uv;

uniform mat4 proj_view_matrix;
uniform mat4 model_matrix;

void main(void) {
  vec4 v = vec4(v_position, 1.0);
  gl_Position = (proj_view_matrix * model_matrix) * v;
  uv = v_uv;
}
