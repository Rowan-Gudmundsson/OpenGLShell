#version 330

layout (location = 0) in vec3 v_position;
layout (location = 2) in vec3 v_normal;

uniform mat4 proj_view_matrix;
uniform mat4 model_matrix;
uniform mat4 depth_mvp;

smooth out vec3 normal;
smooth out vec3 position;
smooth out vec4 shadow_coord;

void main(void) {
  vec4 v = vec4(v_position, 1.0);
  gl_Position = (proj_view_matrix * model_matrix) * v;
  shadow_coord = depth_mvp * v;
  shadow_coord = shadow_coord / shadow_coord.w * 0.5 + vec4(0.5);
  normal = normalize(model_matrix * vec4(v_normal, 0.0)).xyz;
  position = v.xyz;
}
