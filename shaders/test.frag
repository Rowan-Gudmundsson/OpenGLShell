#version 330

smooth in vec3 color;
out vec4 f_color;

void main(void) {
  f_color = vec4(color, 1.0);
}
