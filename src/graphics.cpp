#include "graphics.h"

Graphics::Graphics() {}

bool Graphics::Initialize(int width, int height) {
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

  if (!InitializeCamera(width, height)) {
    std::cout << "Camera failed to initialize." << std::endl;
    return false;
  }

  // No errors, init success
  return true;
}

bool Graphics::InitializeCamera(int width, int height) {
  m_projection_matrix = glm::perspective(
    glm::radians(45.0f), // 45 degree FOV
    float(width) / float(height), // Aspect ratio
    0.01f, // Dist to near plane
    100.0f // Dist to far plane
  );

  glm::vec3 camera_position(5.0f, 5.0f, 0.0f);
  glm::vec3 camera_look_at(0.0f, 0.0f, 0.0f);

  m_view_matrix = glm::lookAt(
    camera_position, // Where the camera is
    camera_look_at, // Where the camera is pointing
    glm::vec3(0.0f, 1.0f, 0.0f) // Which way is up
  );

  // No potential for error here
  return true;
}

void Graphics::AddObject(Object* object) {
  m_objects.push_back(object);
}

void Graphics::Update(unsigned dt) {
  // Itterate through game objects and call the update function
  // on them
  for (auto i : m_objects) {
    i->Update(dt);
  }
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
    // Enable the current shader
    i.second->Enable();

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

  // Itterate through objects and delete
  for (auto i : m_objects) {
    delete i;
    i = nullptr;
  }
}
