#include "graphics.h"

Graphics::Graphics(Options* _options) : options(_options) {}

bool Graphics::Initialize() {
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  //enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable culling for reduced render load and better on
  // worse graphics cards
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  if (!InitializeCamera()) {
    std::cout << "Camera failed to initialize." << std::endl;
    return false;
  }

  // No errors, init success
  return true;
}

bool Graphics::InitializeCamera() {
  m_projection_matrix = glm::perspective(
    options->eye.FOV,
    float(options->window.width) / float(options->window.height), // Aspect ratio
    options->eye.near_plane, // Dist to near plane
    options->eye.far_plane // Dist to far plane
  );

  UpdateCamera();

  // No potential for error here
  return true;
}

void Graphics::AddObject(std::string shader_name, Object* object, bool is_root) {
  Shader* tmp = Shader::LoadShader(shader_name);
  m_shader_list[shader_name] = tmp;

  if (m_render_list.find(shader_name) != m_render_list.end()) {
    m_render_list[shader_name].push_back(object);
  } else {
    std::vector<Object*> temp(1, object);
    m_render_list[shader_name] = temp;
  }
  if (is_root) {
    m_objects.push_back(object);
  }
}

void Graphics::AddPointLight(json light) {
  PointLight point_light(light);
  m_point_lights.push_back(point_light);
}

void Graphics::AddDirectionalLight(json light) {
  DirectionalLight directional_light(light);
  m_directional_lights.push_back(directional_light);
}

void Graphics::Update(unsigned dt) {
  // Itterate through game objects and call the update function
  // on them
  for (auto i : m_objects) {
    i->Update(dt);
  }
}

void Graphics::UpdateCamera() {
  // Rotate <1.0, 0.0, 0.0> around <0.0, 1.0, 0.0> by theta degrees
  options->eye.position = glm::rotate(glm::vec3(1.0, 0.0, 0.0), options->eye.theta, glm::vec3(0.0, 1.0, 0.0));
  // Find the vector perpendicular to <pos.x, pos.y pos.z> and <0.0, 1.0, 0.0>
  glm::vec3 around = glm::cross(options->eye.position, glm::vec3(0.0, 1.0, 0.0));
  // Rotate the posision vector around the perpendicular vector by phi degrees
  options->eye.position = glm::rotate(options->eye.position, options->eye.phi, around);
  // Find the direction of the vector
  options->eye.position = glm::normalize(options->eye.position);
  // And multiply it by the length
  options->eye.position *= options->eye.r;
  // Then shift the origin of the vector to where we're looking
  options->eye.position += options->eye.look_at;

  m_view_matrix = glm::lookAt(
    options->eye.position, // Position of the camera
    options->eye.look_at, // Where the camera is pointing
    glm::vec3(0.0, 1.0, 0.0) // Which direction is up
  );
}

void Graphics::UpdateCamera(float x, float y) {
  // Update our horizontal angle
  options->eye.theta += x * CAMERA_MOVE_DELTA;
  // Update vertical angle
  options->eye.phi += y * CAMERA_MOVE_DELTA / 4;
  // Make sure vertical angle is not 0
  if (options->eye.phi == M_PI / 2 || options->eye.phi == -M_PI / 2) options->eye.phi = options->eye.phi > 0
    ? M_PI / 2 - 0.1f
    : -M_PI / 2 + 0.1f;

  // Update our view matrix
  UpdateCamera();
}

void Graphics::UpdateCamera(int zoom) {
  if (zoom > 0) { // If scrolling up
    // Increase radius
    options->eye.r += CAMERA_ZOOM_DELTA;
  } else if (zoom < 0) { // Else if scrolling down
    // Decrease radius
    options->eye.r -= CAMERA_ZOOM_DELTA;
  }

  // Update our view matrix
  UpdateCamera();
}

void Graphics::Render() {
  // Bind the view buffer
  glBindBuffer(GL_FRAMEBUFFER, 0);

  // Clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Combine projection and view matrices
  glm::mat4 proj_view = m_projection_matrix * m_view_matrix;

  // Itterate through through our shaders
  for (auto i : m_shader_list) {
    // If the shader failed to load
    if (i.second == nullptr) continue;
    // Enable the current shader
    i.second->Enable();

    for (unsigned j = 0; j < m_point_lights.size(); j++) {
      std::string basename = "point_lights[" + std::to_string(j) + "]";
      i.second->uniform3fv(basename + ".light_position", 1, glm::value_ptr(m_point_lights[j].position));
      i.second->uniform3fv(basename + ".light_color", 1, glm::value_ptr(m_point_lights[j].color));
      i.second->uniform1f(basename + ".light_strength", m_point_lights[j].strength);
    }

    for (unsigned j = 0; j < m_directional_lights.size(); j++) {
      std::string basename = "dir_lights[" + std::to_string(j) + "]";
      i.second->uniform3fv(basename + ".light_position", 1, glm::value_ptr(m_directional_lights[j].position));
      i.second->uniform3fv(basename + ".light_direction", 1, glm::value_ptr(m_directional_lights[j].direction));
      i.second->uniform3fv(basename + ".light_color", 1, glm::value_ptr(m_directional_lights[j].color));
      i.second->uniform1f(basename + ".light_strength", m_directional_lights[j].strength);
      i.second->uniform1f(basename + ".outer_angle", m_directional_lights[j].outer_angle);
      i.second->uniform1f(basename + ".inner_angle", m_directional_lights[j].inner_angle);
    }

    // Send uniforms to shader
    i.second->uniformMatrix4fv("proj_view_matrix", 1, GL_FALSE, glm::value_ptr(proj_view));

    // Itterate through all the objects to be rendered
    // by the currently active shader
    for (auto j : m_render_list[i.first]) {
      j->Render(i.second);
    }
  }
}

std::string Graphics::ErrorString(GLenum error) {
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

Graphics::~Graphics() {
  // Itterate through shaders and delete
  for (auto i : m_shader_list) {
    delete i.second;
    i.second = nullptr;
  }
  m_shader_list.clear();

  // Itterate through objects and delete
  for (auto i : m_objects) {
    delete i;
    i = nullptr;
  }
  m_objects.clear();
}
