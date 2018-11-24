#pragma once

#include "object.h"

#define CAMERA_MOVE_DELTA 4.0f
#define CAMERA_ZOOM_DELTA 0.5f

class Graphics {
  public:
    // Constructors
    Graphics(Options*);

    // Setup functions
    bool Initialize();
    bool InitializeCamera();
    void AddObject(std::string, Object*, bool);
    void AddPointLight(json);
    void AddDirectionalLight(json);

    // Runtime function
    void Update(unsigned);
    void UpdateCamera();
    void UpdateCamera(float, float);
    void UpdateCamera(int);
    void Render();

    // Destructors
    ~Graphics();

  private:
    Options* options;
    std::string ErrorString(GLenum);

    std::unordered_map<std::string, std::vector<Object*> > m_render_list;
    std::unordered_map<std::string, Shader*> m_shader_list;

    glm::mat4 m_view_matrix, m_projection_matrix;

    std::vector<Object*> m_objects;
    std::vector<PointLight> m_point_lights;
    std::vector<DirectionalLight> m_directional_lights;
};
