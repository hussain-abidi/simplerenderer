#include "glad/glad.h"
#include <string>

class Shader {
public:
  unsigned int ID;
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  void use();

  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

private:
  void checkErrors(unsigned int shader, std::string type);
};
