#pragma once

#include "model.h"

class Object {
  public:
    // Constructors
    Object(std::string);

    // Runtime functions
    void Update(unsigned);
    void Render(Shader*);

    // Getters
    glm::mat4 GetModel() { return m_model_matrix; }
    glm::vec3 GetPosition() { return m_position; }

    // Destuctors
    ~Object();

  private:
    Model* m_object_model;

    glm::mat4 m_model_matrix;
    glm::vec3 m_position;
};
