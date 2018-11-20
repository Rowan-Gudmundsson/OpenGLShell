#pragma once

#include "shader.h"

#include <Magick++.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#define GL_TEXTURE_POS GL_TEXTURE0
#define GL_NORMAL_POS GL_TEXTURE1

#define GL_TEXTURE_OFFSET 0
#define GL_NORMAL_OFFSET 1

// Constant model path variables
const std::string MODEL_PATH = "../models/";

// Constant texture path variables
const std::string TEXTURE_PATH = "../textures/";

class Texture {
  public:
    // Static functions
    static Texture* LoadTexture(std::string);

    // Constructors
    Texture(std::string);

    // Setup functions
    void InitializeTexture();

    // Runtime functions
    void BindTexture(GLenum);

    // Public member variables
    bool m_initialized;

    // Destructors
    ~Texture();

  private:
    Magick::Blob* t_Blob;
    Magick::Image* t_Image;

    GLuint t_Location;

    bool error;
};

class Model {
  public:
    // Static functions
    static Model* LoadModel(std::string);

    // Constructors
    Model(std::string);

    // Runtime functions
    void DrawModel(Shader*, bool);

    // Public memeber variables
    struct Mesh {
      Mesh() {}
      GLuint VB, IB;
      unsigned num_indices;
      Texture *texture, *normal;
      glm::vec3 ambient, diffuse, specular;
    };

    std::vector<Mesh> m_meshes;

  private:
    void LoadMesh(const aiMesh*, const aiMaterial*);

    bool error;
};
