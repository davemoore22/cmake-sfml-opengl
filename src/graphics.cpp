#include "graphics.hpp"

Graphics::Graphics() {

  program = 0;
  vao = 0;
  vertexVBO = 0;
  indexVBO = 0;
}

Graphics::~Graphics() {}

/// Checks for any errors specific to the shaders. It will output any errors
/// within the shader if it's not valid.
void Graphics::checkError(GLuint l_shader, GLuint l_flag, bool l_program,
                          const std::string &l_errorMsg) {
  GLint success = 0;
  GLchar error[1024] = {0};
  if (l_program) {
    glGetProgramiv(l_shader, l_flag, &success);
  } else {
    glGetShaderiv(l_shader, l_flag, &success);
  }

  if (success) {
    return;
  }
  if (l_program) {
    glGetProgramInfoLog(l_shader, sizeof(error), nullptr, error);
  } else {
    glGetShaderInfoLog(l_shader, sizeof(error), nullptr, error);
  }

  std::cout << l_errorMsg << "\n";
}

/// Creates and compiles a shader.
GLuint Graphics::buildShader(const std::string &l_src, unsigned int l_type) {
  GLuint shaderID = glCreateShader(l_type);
  if (!shaderID) {
    std::cout << "Bad shader type!";
    return 0;
  }
  const GLchar *sources[1];
  GLint lengths[1];
  sources[0] = l_src.c_str();
  lengths[0] = l_src.length();
  glShaderSource(shaderID, 1, sources, lengths);
  glCompileShader(shaderID);
  checkError(shaderID, GL_COMPILE_STATUS, false, "Shader compile error: ");
  return shaderID;
}

/// Function to load the shaders from string data.
void Graphics::LoadFromMemory(const std::string &shaderData, ShaderType type) {
  if (shaderData.empty())
    return;

  if (shader[static_cast<unsigned int>(type)]) {
    glDetachShader(program, shader[static_cast<unsigned int>(type)]);
    glDeleteShader(shader[static_cast<unsigned int>(type)]);
  }

  switch (type) {
  case ShaderType::Vertex:
    shader[static_cast<unsigned int>(type)] =
        buildShader(shaderData, GL_VERTEX_SHADER);
    break;
  case ShaderType::Geometry:
    shader[static_cast<unsigned int>(type)] =
        buildShader(shaderData, GL_GEOMETRY_SHADER);
    break;
  case ShaderType::Fragment:
    shader[static_cast<unsigned int>(type)] =
        buildShader(shaderData, GL_FRAGMENT_SHADER);
    break;
  default:
    break;
  }

  if (program == 0) {
    program = glCreateProgram();
  }

  glAttachShader(program, shader[static_cast<unsigned int>(type)]);
  glBindAttribLocation(program, static_cast<GLuint>(VertexAttribute::Position),
                       "position");
  glBindAttribLocation(program, static_cast<GLuint>(VertexAttribute::TexCoord),
                       "texCoord");

  glLinkProgram(program);
  checkError(program, GL_LINK_STATUS, true, "Shader link error:");
  glValidateProgram(program);
  checkError(program, GL_VALIDATE_STATUS, true, "Invalid shader:");

  uniform[static_cast<unsigned int>(UniformType::TransformPVM)] =
      glGetUniformLocation(program, "pvm");
}
