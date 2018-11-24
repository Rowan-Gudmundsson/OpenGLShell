#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_uv;

uniform mat4 proj_view_matrix;
uniform mat4 model_matrix;

smooth out vec3 color;

void main(void) {
  vec4 v = vec4(v_position, 1.0);

  gl_Position = (proj_view_matrix * model_matrix) * v;

  color.r = (sin(v_uv.x) + 1) / 2;
  color.g = (sin(v_uv.y) + 1) / 2;
  color.b = (sin(v_uv.x) + 1) / 2;
}
