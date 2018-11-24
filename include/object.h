#pragma once

#include "model.h"

class Object {
  public:
    // Constructors
    Object(json, Options*);

    // Initialize functions
    void AddChild(Object*);

    // Runtime functions
    void Update(unsigned);
    void Render(Shader*);

    // Getters
    glm::mat4 GetModel() { return m_model_matrix; }
    glm::vec3 GetPosition() { return m_position; }

    // Public data members
    ObjectProps props;

    // Destuctors
    ~Object();

  private:
    Options* options;
    Model* m_object_model;

    glm::mat4 m_model_matrix;
    glm::vec3 m_position;

    std::vector<Object*> children;
};
