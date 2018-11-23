#version 330

smooth in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;

void main(void) {
  // vec3 tex_color = texture(texture_sampler, uv).xyz;
  vec3 tex_color = vec3(0.0, 1.0, 0.5);
  color = vec4(tex_color, 1.0);
}
