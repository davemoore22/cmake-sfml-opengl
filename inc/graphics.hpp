#pragma once

#include "main.hpp"

class Graphics {

public:
  Graphics();
  ~Graphics();

  void checkError(GLuint l_shader, GLuint l_flag, bool l_program,
                  const std::string &l_errorMsg);
  GLuint buildShader(const std::string &l_src, unsigned int l_type);
  void LoadFromMemory(const std::string &shaderData, ShaderType type);

  GLuint program;

  /// List of shaders set up for a 3D scene.
  GLuint shader[static_cast<unsigned int>(ShaderType::Count)];
  /// List of uniforms that can be defined values for the shader.
  GLint uniform[static_cast<unsigned int>(UniformType::Count)];

  /// Vertex Array Object ID.
  GLuint vao;
  /// Vertex Buffer Object for Vertices.
  GLuint vertexVBO;
  /// Vertex Buffer Object for Indices.
  GLuint indexVBO;
  /// Depending on input, the amount of vertices or indices that are needed to
  /// be drawn for this object.
  unsigned int drawCount;

private:
};
