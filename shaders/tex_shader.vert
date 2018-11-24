#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_uv;
layout (location = 2) in vec3 v_normal;

uniform mat4 proj_view_matrix;
uniform mat4 model_matrix;

smooth out vec3 normal;
smooth out vec2 uv;
smooth out vec3 position;

void main(void) {
  vec4 v = vec4(v_position, 1.0);
  gl_Position = (proj_view_matrix * model_matrix) * v;

  uv = v_uv;
  normal = normalize(model_matrix * vec4(v_normal, 0.0)).xyz;
  position = (model_matrix * v).xyz;
}
