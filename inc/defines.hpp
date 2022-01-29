#include <string>

const std::string defaultVertexShader =
    "#version 330\n"
    "attribute vec3 position;"
    "attribute vec2 texCoord;"
    "uniform mat4 pvm;"

    "varying vec2 uv;"

    "void main() {"
    "	gl_Position = pvm * vec4(position, 1.0);"
    "	uv = texCoord;"
    "}";

/// Basic fragment shader that returns the colour of a pixel based on the input
/// texture and its coordinate.
const std::string defaultFragShader = "#version 330\n"
                                      "uniform sampler2D texture;"
                                      "varying vec2 uv;"

                                      "void main() {"
                                      "	gl_FragColor = texture2D(texture, uv);"
                                      "}";

/// Shader Types
enum class ShaderType { Vertex, Fragment, Geometry, Count };
/// Standard Uniforms in the shader.
enum class UniformType { TransformPVM, Count };
/// Vertex attributes for shaders and the input vertex array.
enum class VertexAttribute { Position, TexCoord, COUNT };
