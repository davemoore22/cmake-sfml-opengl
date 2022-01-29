#include "main.hpp"
#include "graphics.hpp"

// Note that in VSCode, run and debug/run from command line works

// Code from Tim Stoddard's rewrite of the SFML opengl tutorial (needs
// optimation - its a quick and dirty adaptation)

// https://www.sfml-dev.org/tutorials/2.5/window-opengl.php and
// https://gist.github.com/gamepopper/1931ca297f3decdee90e785f12762192
int main() {
  bool exit = false;
  bool sRgb = false;

  std::unique_ptr<Graphics> graphics = std::make_unique<Graphics>();

  while (!exit) {
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = sRgb;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600),
                            "SFML graphics with OpenGL", sf::Style::Default,
                            contextSettings);
    window.setVerticalSyncEnabled(true);

    // Initialise GLEW for the extended functions.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
      return EXIT_FAILURE;

    // Create a sprite for the background
    sf::Texture backgroundTexture;
    backgroundTexture.setSrgb(sRgb);
    if (!backgroundTexture.loadFromFile("background.jpg"))
      return EXIT_FAILURE;
    sf::Sprite background(backgroundTexture);

    // Create some text to draw on top of our OpenGL object
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf"))
      return EXIT_FAILURE;
    sf::Text text("SFML / OpenGL demo", font);
    sf::Text sRgbInstructions("Press space to toggle sRGB conversion", font);
    sf::Text mipmapInstructions("Press return to toggle mipmapping", font);
    text.setFillColor(sf::Color(255, 255, 255, 170));
    sRgbInstructions.setFillColor(sf::Color(255, 255, 255, 170));
    mipmapInstructions.setFillColor(sf::Color(255, 255, 255, 170));
    text.setPosition(250.f, 450.f);
    sRgbInstructions.setPosition(150.f, 500.f);
    mipmapInstructions.setPosition(180.f, 550.f);

    // Load a texture to apply to our 3D cube
    sf::Texture texture;
    if (!texture.loadFromFile("texture.jpg"))
      return EXIT_FAILURE;

    // Attempt to generate a mipmap for our cube texture
    // We don't check the return value here since
    // mipmapping is purely optional in this example
    texture.generateMipmap();

    // Make the window the active window for OpenGL calls
    window.setActive(true);

    // Load the shaders we need.
    if (graphics->program == 0) {
      graphics->LoadFromMemory(defaultVertexShader, ShaderType::Vertex);
      graphics->LoadFromMemory(defaultFragShader, ShaderType::Fragment);
    }

    // Enable Z-buffer read and write and culling.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Setup a perspective projection
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    glm::mat4 projection = glm::frustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.0f);

    // Define a 3D cube (6 faces made of 2 triangles composed by 3 indices of a
    // list of vertices)
    static const GLfloat cube[] = {
        // positions   // texture coordinates
        // front
        -20,
        -20,
        -20,
        0,
        0,
        20,
        -20,
        -20,
        1,
        0,
        20,
        20,
        -20,
        1,
        1,
        -20,
        20,
        -20,
        0,
        1,
        // right
        20,
        20,
        -20,
        1,
        1,
        20,
        20,
        20,
        0,
        1,
        20,
        -20,
        20,
        0,
        0,
        20,
        -20,
        -20,
        1,
        0,
        // back
        -20,
        -20,
        20,
        0,
        0,
        20,
        -20,
        20,
        1,
        0,
        20,
        20,
        20,
        1,
        1,
        -20,
        20,
        20,
        0,
        1,
        // left
        -20,
        -20,
        20,
        0,
        0,
        -20,
        -20,
        -20,
        1,
        0,
        -20,
        20,
        -20,
        1,
        1,
        -20,
        20,
        20,
        0,
        1,
        // upper
        20,
        -20,
        20,
        0,
        1,
        -20,
        -20,
        20,
        1,
        1,
        -20,
        -20,
        -20,
        1,
        0,
        20,
        -20,
        -20,
        0,
        0,
        // bottom
        -20,
        20,
        -20,
        0,
        1,
        20,
        20,
        -20,
        1,
        1,
        20,
        20,
        20,
        1,
        0,
        -20,
        20,
        20,
        0,
        0,
    };

    // Define indices for 3D cube.
    static const unsigned int indices[] = {
        2,  1,  0,  3,  2,  0,  // front
        4,  5,  6,  4,  6,  7,  // right
        8,  9,  10, 8,  10, 11, // back
        14, 13, 12, 15, 14, 12, // left
        16, 17, 18, 16, 18, 19, // upper
        22, 21, 20, 23, 22, 20  // bottom
    };

    // Stride is the number of bytes per array element.
    auto stride = sizeof(GLfloat) * 5;
    // Data offset for texture coordinate in bytes.
    auto textureCoordOffset = sizeof(GLfloat) * 3;
    // Amount of index elements in total.
    graphics->drawCount = sizeof(indices) / sizeof(unsigned int);

    // Generate Vertex Array and Vertex Buffer and point the area of data to
    // assign to each attribute.
    glGenVertexArrays(1, &graphics->vao);
    glBindVertexArray(graphics->vao);
    glGenBuffers(1, &graphics->vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, graphics->vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, graphics->drawCount * stride, cube,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));
    glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 3,
                          GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TexCoord));
    glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TexCoord), 2,
                          GL_FLOAT, GL_FALSE, stride,
                          (void *)textureCoordOffset);

    // Generate Index Buffer and define the amount of indices to point to.
    glGenBuffers(1, &graphics->indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphics->indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 graphics->drawCount * sizeof(indices[0]), indices,
                 GL_STATIC_DRAW);

    // Make sure to bind the vertex array to null if you wish to define more
    // objects.
    glBindVertexArray(0);

    // Make the window no longer the active window for OpenGL calls
    window.setActive(false);

    // Create a clock for measuring the time elapsed
    sf::Clock clock;

    // Flag to track whether mipmapping is currently enabled
    bool mipmapEnabled = true;

    // Start game loop
    while (window.isOpen()) {
      // Process events
      sf::Event event;
      while (window.pollEvent(event)) {
        // Close window: exit
        if (event.type == sf::Event::Closed) {
          exit = true;
          window.close();
        }

        // Escape key: exit
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Escape)) {
          exit = true;
          window.close();
        }

        // Return key: toggle mipmapping
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Return)) {
          if (mipmapEnabled) {
            // We simply reload the texture to disable mipmapping
            if (!texture.loadFromFile("texture.jpg"))
              return EXIT_FAILURE;

            mipmapEnabled = false;
          } else {
            texture.generateMipmap();

            mipmapEnabled = true;
          }
        }

        // Space key: toggle sRGB conversion
        if ((event.type == sf::Event::KeyPressed) &&
            (event.key.code == sf::Keyboard::Space)) {
          sRgb = !sRgb;
          window.close();
        }

        // Adjust the viewport when the window is resized
        if (event.type == sf::Event::Resized) {
          // Make the window the active window for OpenGL calls
          window.setActive(true);

          glViewport(0, 0, event.size.width, event.size.height);

          // Make the window no longer the active window for OpenGL calls
          window.setActive(false);
        }
      }

      // Clear the depth buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Configure the viewport (the same size as the window)
      glViewport(0, 0, window.getSize().x, window.getSize().y);

      // Draw the background
      window.pushGLStates();
      window.draw(background);
      window.popGLStates();

      // Make the window the active window for OpenGL calls
      window.setActive(true);

      // Bind the texture
      sf::Texture::bind(&texture);
      glBindVertexArray(graphics->vao);

      // We get the position of the mouse cursor, so that we can move the box
      // accordingly
      float x =
          sf::Mouse::getPosition(window).x * 200.f / window.getSize().x - 100.f;
      float y = -sf::Mouse::getPosition(window).y * 200.f / window.getSize().y +
                100.f;

      // Apply some transformations
      glm::mat4 matrix_pos = glm::translate(glm::vec3(x, y, -100.f));
      glm::mat4 matrix_rotX = glm::rotate(clock.getElapsedTime().asSeconds() *
                                              50.f * (3.1415926f / 180.0f),
                                          glm::vec3(1.f, 0.f, 0.f));
      glm::mat4 matrix_rotY = glm::rotate(clock.getElapsedTime().asSeconds() *
                                              30.f * (3.1415926f / 180.0f),
                                          glm::vec3(0.f, 1.f, 0.f));
      glm::mat4 matrix_rotZ = glm::rotate(clock.getElapsedTime().asSeconds() *
                                              90.f * (3.1415926f / 180.0f),
                                          glm::vec3(0.f, 0.f, 1.f));

      glm::mat4 matrix_rotation = matrix_rotZ * matrix_rotY * matrix_rotX;
      glm::mat4 transform = matrix_pos * matrix_rotation;

      glm::mat4 identity;
      glm::mat4 viewProj = projection * transform;

      // Bind the shaders.
      glUseProgram(graphics->program);

      // Set the uniforms for the shader to use.
      if (graphics->uniform[(int)UniformType::TransformPVM] >= 0)
        glUniformMatrix4fv(
            (unsigned int)graphics->uniform[(int)UniformType::TransformPVM], 1,
            GL_FALSE, &viewProj[0][0]);

      // Draw the cube
      glDrawElements(GL_TRIANGLES, graphics->drawCount, GL_UNSIGNED_INT, 0);

      // Reset the vertex array bound, shader and texture for other assets to
      // draw.
      glBindVertexArray(0);
      glUseProgram(0);
      glBindTexture(GL_TEXTURE_2D, 0);

      // Make the window no longer the active window for OpenGL calls
      window.setActive(false);

      // Draw some text on top of our OpenGL object
      window.pushGLStates();
      window.draw(text);
      window.draw(sRgbInstructions);
      window.draw(mipmapInstructions);
      window.popGLStates();

      // Finally, display the rendered frame on screen
      window.display();
    }

    // Destroy all buffers, shaders and programs.
    glDeleteBuffers(1, &graphics->vertexVBO);
    glDeleteBuffers(1, &graphics->indexVBO);
    glDeleteVertexArrays(1, &graphics->vao);

    // Setting these values to zero will allow them to be initialised with new
    // data on reset.
    graphics->vertexVBO = 0;
    graphics->indexVBO = 0;
    graphics->vao = 0;

    for (unsigned int i = 0; i < static_cast<unsigned int>(ShaderType::Count);
         i++) {
      glDetachShader(graphics->program, graphics->shader[i]);
      glDeleteShader(graphics->shader[i]);
      graphics->shader[i] = 0;
    }

    for (unsigned int i = 0; i < static_cast<unsigned int>(UniformType::Count);
         i++) {
      graphics->uniform[i] = -1;
    }

    glDeleteProgram(graphics->program);
    graphics->program = 0;
  }

  return EXIT_SUCCESS;
}