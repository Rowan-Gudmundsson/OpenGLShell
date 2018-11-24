#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "json.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

using json = nlohmann::json;
extern json config;

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
#endif

// GLM for matricies
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct PointLight {
  PointLight(json light):
      strength(light["LIGHT_STRENGTH"].get<float>()) {
    position.x = light["LIGHT_POSITION"][0].get<float>();
    position.y = light["LIGHT_POSITION"][1].get<float>();
    position.z = light["LIGHT_POSITION"][2].get<float>();
    color.x = light["LIGHT_COLOR"][0].get<float>();
    color.y = light["LIGHT_COLOR"][1].get<float>();
    color.z = light["LIGHT_COLOR"][2].get<float>();
  }
  glm::vec3 position, color;
  float strength;
};

struct DirectionalLight {
  DirectionalLight(json light):
      strength(light["LIGHT_STRENGTH"].get<float>()),
      outer_angle(light["OUTER_ANGLE"].get<float>()),
      inner_angle(light["INNER_ANGLE"].get<float>()) {
    position.x = light["LIGHT_POSITION"][0].get<float>();
    position.y = light["LIGHT_POSITION"][1].get<float>();
    position.z = light["LIGHT_POSITION"][2].get<float>();
    direction.x = light["LIGHT_DIRECTION"][0].get<float>();
    direction.y = light["LIGHT_DIRECTION"][1].get<float>();
    direction.z = light["LIGHT_DIRECTION"][2].get<float>();
    color.x = light["LIGHT_COLOR"][0].get<float>();
    color.y = light["LIGHT_COLOR"][1].get<float>();
    color.z = light["LIGHT_COLOR"][2].get<float>();
  }
  glm::vec3 position, direction, color;
  float strength, outer_angle, inner_angle;
};

struct Vertex {
  Vertex() {}
  Vertex(glm::vec3 p, glm::vec2 u, glm::vec3 n, glm::vec3 t, glm::vec3 b) : position(p), uv(u), normal(n), tangent(t), bitangent(b) {}
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct Options {
  Options(json conf) :
    eye(conf["EYE"]) {}
  struct Eye {
    Eye(json eye_conf) :
        FOV(eye_conf["FOV"].get<float>()),
        near_plane(eye_conf["NEAR_PLANE"].get<float>()),
        far_plane(eye_conf["FAR_PLANE"].get<float>()) {
      theta = eye_conf["TPR"][0].get<float>();
      phi = eye_conf["TPR"][1].get<float>();
      r = eye_conf["TPR"][2].get<float>();
      look_at = glm::vec3(0.0f);
    }
    glm::vec3 position, look_at;
    float FOV, near_plane, far_plane;
    float theta, phi, r;
  } eye;
  struct Window {
    std::string name;
    int width, height;
    bool fullscreen;
  } window;
};

struct ObjectProps {
  ObjectProps(json obj):
    name(obj["NAME"].get<std::string>()),
    transform(obj["TRANSFORM"]),
    model_name(obj["MODEL"].get<std::string>()),
    shader_name(obj["SHADER"].get<std::string>()) {}
  std::string name;
  struct Transform {
    Transform(json trans):
        scale(trans["SCALE"].get<float>()),
        collision_type(trans["COLLISION_TYPE"].get<unsigned>()),
        mesh_type(trans["MESH_TYPE"].get<unsigned>()) {
      rotation.x = trans["ROTATION"][0].get<float>();
      rotation.y = trans["ROTATION"][1].get<float>();
      rotation.z = trans["ROTATION"][2].get<float>();
      position.x = trans["POSITION"][0].get<float>();
      position.y = trans["POSITION"][1].get<float>();
      position.z = trans["POSITION"][2].get<float>();
    }
    float scale;
    glm::vec3 rotation, position;
    unsigned collision_type, mesh_type;
  } transform;
  std::string model_name, shader_name;
};

std::string load_file(std::string);
std::string clip_path(std::string);
