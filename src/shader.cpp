#include "shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexShaderFile.open(vertexPath);
    fragmentShaderFile.open(fragmentPath);

    std::stringstream vertexShaderStream, fragmentShaderStream;

    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "Error reading shader file" << '\n';
  }

  const char *vertexShaderCode = vertexCode.c_str();
  const char *fragmentShaderCode = fragmentCode.c_str();

  unsigned int vertex, fragment;

  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
  glCompileShader(vertex);

  checkErrors(vertex, "vertex");

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
  glCompileShader(fragment);

  checkErrors(fragment, "fragment");

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  checkErrors(ID, "program");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkErrors(unsigned int shader, std::string type) {
  int success;
  char infoLog[512];

  if (type == "program") {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "Error linking program: " << infoLog << '\n';
    }
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "Error linking " << type << " shader: " << infoLog << '\n';
    }
  }
}
