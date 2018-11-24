#version 330

#define POINT_COUNT 6
#define DIRECTIONAL_COUNT 3

struct PointLight {
  vec3 light_position;
  vec3 light_color;
  float light_strength;
};

struct DirectionalLight {
  vec3 light_position;
  vec3 light_direction;
  vec3 light_color;
  float light_strength;
  float inner_angle;
  float outer_angle;
};

smooth in vec3 normal;
smooth in vec3 position;
smooth in vec4 shadow_coord;

uniform vec3 eye_position;

uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform uint refractive_index;

uniform PointLight point_lights[POINT_COUNT];
uniform DirectionalLight dir_lights[DIRECTIONAL_COUNT];

uniform sampler2D shadow_map;

out vec4 f_color;

void main(void) {
  float bias = 0.005;
  vec3 ambient = 0.2 * ambient_color;
  vec3 diffuse = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);
  float visibility = 1.0;

  float depth = texture(shadow_map, shadow_coord.xy).r;
  if (depth < shadow_coord.z) {
    visibility = 0.5;
  }

  for (int i = 0; i < POINT_COUNT; i++) {
    vec3 light_direction = normalize(point_lights[i].light_position - position);
    float dist = distance(point_lights[i].light_position, position);
    vec3 view_direction = normalize(eye_position - position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    // Diffuse
    diffuse += point_lights[i].light_strength / dist * diffuse_color * max(dot(normal, light_direction), 0.0) * point_lights[i].light_color;
    // Specular
    diffuse += point_lights[i].light_strength / dist * pow(max(dot(view_direction, reflect_direction), 0.0), refractive_index) * point_lights[i].light_color;
  }

  for (int i = 0; i < DIRECTIONAL_COUNT; i++) {
    vec3 point_direction = normalize(position - dir_lights[i].light_position);
    float dist = distance(dir_lights[i].light_position, position);
    float theta = dot(point_direction, normalize(dir_lights[i].light_direction));
    float intensity = theta < 0
      ? 0.0f
      : clamp((acos(theta) - dir_lights[i].outer_angle) / (dir_lights[i].inner_angle - dir_lights[i].outer_angle), 0.0, 1.0);
    diffuse += dir_lights[i].light_strength / dist * intensity * dir_lights[i].light_color * max(dot(normal, normalize(dir_lights[i].light_position - position)), 0.0);
  }

  vec3 color = (ambient + (specular + diffuse)) * diffuse_color;
  // vec3 color = depth;
  f_color = vec4(color, 1.0);
}
