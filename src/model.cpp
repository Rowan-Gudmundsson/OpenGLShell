#include "model.h"


/* ------------------------------------------------------------
 * Texture Class - For loading textures
 * -----------------------------------------------------------*/
Texture* Texture::LoadTexture(std::string texture_name) {
  // So we don't load the same texture more than once
  static std::unordered_map<std::string, Texture*> texture_map;

  // If the texture already exists in the map return it
  if (texture_map.find(texture_name) != texture_map.end()) {
    return texture_map[texture_name];
  }

  // Else create a new texture
  Texture* new_texture = new Texture(texture_name);

  // If an error occurred return nullptr
  if (new_texture->error) {
    return nullptr;
  }

  // Else insert the new texture into the map and return it
  texture_map[texture_name] = new_texture;
  return new_texture;
}

Texture::Texture(std::string texture_name) {
  error = false;

  try {
    // Load image
    t_Image = new Magick::Image(TEXTURE_PATH + texture_name);
    t_Blob = new Magick::Blob();
    t_Image->magick("JPEG");
    // Write image data to blob
    t_Image->write(t_Blob, "RGBA");
  } catch(Magick::Error& err) {
    std::cout << "Failed to load texture " << texture_name <<  ", Error: " << err.what() << std::endl;
    error = true;
  }

  initialized = false;
}

void Texture::InitializeTexture() {
  if (!initialized) {
    // Gen new texture location
    glGenTextures(1, &t_Location);
    // Bind newly generated texture to active
    glBindTexture(GL_TEXTURE_2D, t_Location);

    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      t_Image->columns(),
      t_Image->rows(),
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      t_Blob->data()
    );

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete t_Image;
    delete t_Blob;

    t_Image = nullptr;
    t_Blob = nullptr;

    initialized = true;
  }
}

void Texture::BindTexture(GLenum t_Target) {
  glActiveTexture(t_Target);
  glBindTexture(GL_TEXTURE_2D, t_Location);
}

Texture::~Texture() {
  if (!initialized) {
    delete t_Image;
    delete t_Blob;
    t_Image = nullptr;
    t_Blob = nullptr;
  }
}

/* ------------------------------------------------------------
 * Model Class - For loading models
 * -----------------------------------------------------------*/
Model* Model::LoadModel(std::string model_name) {
  // So we dont load the same model more than once
  static std::unordered_map<std::string, Model*> model_map;

  // If the model is already loaded return a pointer to it
  if (model_map.find(model_name) != model_map.end()) {
    return model_map[model_name];
  }

  // Else create a new model
  Model* new_model = new Model(model_name);

  // If an error occured return nullptr
  if (new_model->error) {
    return nullptr;
  }

  // Else insert the new model into the map and return a pointer
  // to the new model
  model_map[model_name] = new_model;
  return new_model;
}

Model::Model(std::string model_name) {
  error = false;
  // Import the image from the file
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    MODEL_PATH + model_name,
    aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs
  );

  if (scene) { // If there were no errors load the meshes
    aiMesh* mesh;
    aiMaterial* material;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
      mesh = scene->mMeshes[i];
      material = scene->mMaterials[mesh->mMaterialIndex];
      LoadMesh(mesh, material);
    }
  } else { // Else set error flag
    std::cout << "Error loading scene, " << importer.GetErrorString() << std::endl;
    error = true;
  }
}

void Model::DrawModel(Shader* shader, bool draw_complex) {
  // Loop through meshes
  for (auto i : meshes) {
    // Enable attribute pointers
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, i.VB);

    // Give offsets for attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    if (draw_complex) {
      // Pass uniforms
      shader->uniform3fv("ambient_color", 1, glm::value_ptr(i.ambient));
      shader->uniform3fv("diffuse_color", 1, glm::value_ptr(i.diffuse));
      shader->uniform3fv("specular_color", 1, glm::value_ptr(i.specular));

      // Pass texture
      if (i.texture != nullptr) {
        i.texture->BindTexture(GL_TEXTURE_POS);
        shader->uniform1i("texture_sampler", GL_TEXTURE_OFFSET);
      }

      // Pass normal
      if (i.normal != nullptr) {
        i.normal->BindTexture(GL_NORMAL_POS);
        shader->uniform1i("normal_sampler", GL_NORMAL_OFFSET);
      }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IB);

    // Draw the triagles
    glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Set active texture to nothing
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Model::LoadMesh(const aiMesh* mesh, const aiMaterial* material) {
  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  Model::Mesh new_mesh;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D& position = mesh->mVertices[i];
    aiVector3D& normal = mesh->mNormals[i];

    if (mesh->mTextureCoords[0]) {
      aiVector3D& tex = mesh->mTextureCoords[0][i];
      Vertex v(glm::vec3(position.x, position.y, position.z),
               glm::vec2(tex.x, tex.y),
               glm::vec3(normal.x, normal.y, normal.z));
      Vertices.push_back(v);
    } else {
      Vertex v(glm::vec3(position.x, position.y, position.z),
               glm::vec2(0.0f, 0.0f),
               glm::vec3(normal.x, normal.y, normal.z));
      Vertices.push_back(v);
    }
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    const aiFace& face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
    {
      Indices.push_back(face.mIndices[j]);
    }
  }

  aiColor3D amb, diff, spec;
  material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
  material->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
  material->Get(AI_MATKEY_COLOR_SPECULAR, spec);

  new_mesh.ambient_color = glm::vec3(amb.r, amb.g, amb.b);
  new_mesh.diffuse_color = glm::vec3(diff.r, diff.g, diff.b);
  new_mesh.specular_color = glm::vec3(spec.r, spec.g, spec.b);

  {
    aiString pathname;
    material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), pathname);
    std::string filename = clip_path(pathname.C_Str());

    if (filename != "") {
      new_mesh.texture = Texture::LoadTexture(filename);
    } else {
      new_mesh.texture = nullptr;
    }
    if (new_mesh.texture != nullptr) {
      new_mesh.texture->InitializeTexture();
    }
  }
  {
    aiString pathname;
    material->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), pathname);
    std::string filename = clip_path(pathname.C_Str());

    if (filename != "") {
      new_mesh.normal = Texture::LoadTexture(filename);
    } else {
      new_mesh.normal = nullptr;
    }
    if (new_mesh.normal != nullptr) {
      new_mesh.normal->InitializeTexture();
    }
  }

  glGenBuffers(1, &new_mesh.VB);
  glBindBuffer(GL_ARRAY_BUFFER, new_mesh.VB);
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(Vertex) * Vertices.size(),
    &Vertices[0],
    GL_STATIC_DRAW
  );

  glGenBuffers(1, &new_mesh.IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh.IB);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(unsigned int) * Indices.size(),
    &Indices[0],
    GL_STATIC_DRAW
  );

  new_mesh.num_indices = Indices.size();
  meshes.push_back(new_mesh);
}
