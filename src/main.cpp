#include "glad/glad.h"

#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>

GLFWwindow *window = nullptr;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

bool init(int width, int height, const std::string &title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 0);

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create GLFW window" << '\n';
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << '\n';
    return false;
  }

  glViewport(0, 0, width, height);

  return true;
}

void initRender(unsigned int &VAO, unsigned int &EBO, unsigned int &VBO) {
  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      -0.5f, -0.5f, 0.0f, // bottom right
      0.0f,  0.5f,  0.0f, // bottom left
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
}

void render(unsigned int VAO, Shader &shader) {
  shader.use();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void loop() {
  unsigned int VAO, VBO, EBO;
  initRender(VAO, VBO, EBO);

  Shader shader("shaders/shader.vert", "shaders/shader.frag");

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // render
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render(VAO, shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

int main() {
  if (!init(800, 600, "SimpleRenderer")) {
    return 1;
  }

  loop();

  glfwTerminate();
  return 0;
}
