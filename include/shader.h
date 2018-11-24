#pragma once

#include "graphics_headers.h"

const std::string SHADER_PATH = "../shaders/";

class Shader {
  public:
    // Static functions
    static Shader* LoadShader(std::string);

    // Constructors
    Shader();

    // Setup functions
    bool Initialize();
    void Enable();
    bool AddShader(GLenum, std::string);
    bool Finalize();

    // Uniform functions
    void uniform1i(const std::string&, GLint);
    void uniform1f(const std::string&, GLfloat);
    void uniform3fv(const std::string&, GLsizei, const GLfloat*);
    void uniformMatrix4fv(const std::string&, GLsizei, GLboolean, const GLfloat*);

    // Destructors
    ~Shader();

  private:
    std::string m_shader_name;
    GLuint m_shader_program;
    std::vector<GLuint> m_shader_object_list;

    GLint GetUniformLocation(const std::string&);
};
