#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// Vertex shader source code
const char *vertexShaderSource = R"glsl(
    #version 410 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
)glsl";

// Fragment shader source code
const char *fragmentShaderSource = R"glsl(
    #version 410 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)glsl";

// Error checking
void checkShaderCompile(GLuint shader, const std::string &type) {
  GLint success;
  GLchar infoLog[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
    std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
              << infoLog << "\n";
  }
}

int main() {
  // Initialize GLFW
  if (!glfwInit())
    return -1;

  // Request OpenGL 4.1 core profile on macOS
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS

  GLFWwindow *window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Load OpenGL functions via GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  // Viewport setup
  glViewport(0, 0, 800, 600);

  // Triangle vertices
  float vertices[] = {
      0.0f,  0.5f,  0.0f, // top
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f  // right
  };

  // Vertex Array Object and Vertex Buffer Object
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  // Upload vertex data
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Compile shaders
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  checkShaderCompile(vertexShader, "VERTEX");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  checkShaderCompile(fragmentShader, "FRAGMENT");

  // Create shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Delete shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}
