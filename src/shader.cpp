#include "shader.h"

Shader* LoadShader(std::string shader_name) {
  // So we dont load the same shader more than once
  static std::unordered_map<std::string, Shader*> shader_map;

  // If we have already loaded the shader return a pointer to it
  if (shader_map.find(shader_name) != shader_map.end()) {
    return shader_map[shader_name];
  }

  // Else create a new shader
  Shader* new_shader = new Shader();

  // Set up the shader program
  if(!new_shader->Initialize()) {
    std::cout << "Failed to initialize shader." << std::endl;
    return nullptr;
  }

  // Add the vertex shader
  if(!new_shader->AddShader(GL_VERTEX_SHADER, shader_name)) {
    std::cout << "Vertex shader failed to initialize." << std::endl;
    return nullptr;
  }

  // Add the fragment shader
  if(!new_shader->AddShader(GL_FRAGMENT_SHADER, shader_name)) {
    std::cout << "Fragment shader failed to initialize." << std::endl;
    return nullptr;
  }

  // Connect the program
  if(!new_shader->Finalize()) {
    std::cout << "Program failed to finalize." << std::endl;
    return nullptr;
  }

  // Insert the new shader into the map and return a pointer
  // to it.
  shader_map[shader_name] = new_shader;
  return new_shader;
}

Shader::Shader() :  m_shader_program(0) {}

bool Shader::Initialize()
{
  m_shader_program = glCreateProgram();

  if (m_shader_program == 0)
  {
    std::cout << "Error creating shader program\n";
    return false;
  }

  return true;
}

void Shader::Enable()
{
    glUseProgram(m_shader_program);
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, std::string shader_name)
{
  std::string s;

  if(ShaderType == GL_VERTEX_SHADER)
  {
    s = load_file(SHADER_PATH + shader_name + std::string(".vert")).c_str();
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    s = load_file(SHADER_PATH + shader_name + std::string(".frag")).c_str();
  }

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0)
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shader_object_list.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shader_program, ShaderObj);

  return true;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shader_program);

  glGetProgramiv(m_shader_program, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shader_program, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shader_program);
  glGetProgramiv(m_shader_program, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shader_program, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (auto it : m_shader_object_list)
  {
    glDeleteShader(it);
  }

  m_shader_object_list.clear();

  return true;
}

void uniformMatrix4fv(const std::string& name, GLsizei size, GLboolean transpose, const GLfloat* value) {
  GLint location = GetUniformLocation(name);

  if (location == INVALID_UNIFORM_LOCATION) {
    std::cout << "Invalid location of uniform (" << name << ")" << std::endl;
    return;
  }

  glUniformMatrix4fv(location, size, transpose, value);
}

GLint Shader::GetUniformLocation(const std::string& pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName.c_str());

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName.c_str());
    }

    return Location;
}

Shader::~Shader() {
  for (auto it : m_shader_object_list)
  {
    glDeleteShader(it);
  }

  if (m_shader_program != 0)
  {
    glDeleteProgram(m_shader_program);
    m_shader_program = 0;
  }
}
