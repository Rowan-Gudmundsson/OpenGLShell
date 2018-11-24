#include "object.h"

Object::Object(json _props, Options* _options) : props(_props), options(_options) {
  // If object has a model, load it
  if (props.model_name != "") {
    m_object_model = Model::LoadModel(props.model_name);
  }
}

void Object::AddChild(Object* child) {
  children.push_back(child);
}

void Object::Update(unsigned dt) {
  m_model_matrix = glm::mat4(1.0f);
}

void Object::Render(Shader* shader) {
  shader->uniformMatrix4fv("model_matrix", 1, GL_FALSE, glm::value_ptr(m_model_matrix));
  m_object_model->DrawModel(shader, true);
}

Object::~Object() {
  for (auto i : children) {
    delete i;
  }
  children.clear();
}
