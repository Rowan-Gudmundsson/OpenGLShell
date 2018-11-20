#pragma once

#include "object.h"

class Graphics {
  public:
    // Constructors
    Graphics();

    // Setup functions
    bool Initialize(int, int);
    bool InitializeCamera(int, int);
    void AddObject(Object*);

    // Runtime function
    void Update(unsigned);
    void Render();

    // Destructors
    ~Graphics();

  private:
    std::string ErrorString(GLenum);

    std::unordered_map<std::string, std::vector<Object*> > m_render_list;
    std::unordered_map<std::string, Shader*> m_shader_list;

    glm::mat4 m_view_matrix, m_projection_matrix;

    std::vector<Object*> m_objects;
};
