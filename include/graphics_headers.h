#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
  #define GLM_ENABLE_EXPERIMENTAL
#else //linux as default
  #include <GL/glew.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex {
  Vertex() {}
  Vertex(glm::vec3 p, glm::vec2 u, glm::vec3 n) : position(p), uv(u), normal(n) {}
  glm::vec2 uv;
  glm::vec3 position, normal;
};

std::string load_file(std::string);
std::string clip_path(std::string);
