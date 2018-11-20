#include "object.h"

Object::Object(std::string model_name) {
  // If object has a model, load it
  if (model_name != "") {
    m_object_model = Model::LoadModel(model_name);
  }
}

void Object::Update(unsigned dt) {
  m_model_matrix = glm::mat4(1.0f);
}

void Object::Render(Shader* shader) {
  shader->uniformMatrix4fv("model_matrix", 1, GL_FALSE, glm::value_ptr(m_model_matrix));
  m_object_model->DrawModel(shader, true);
}

Object::~Object() {}
