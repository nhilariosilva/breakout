#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

class Shader{
public:
  GLuint ID;
  Shader();
  ~Shader();

  void compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar* geometryCode = nullptr);
  Shader &use();

  void setInteger(const GLchar* name, GLuint value, GLboolean useShader = false);
  void setFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
  void setVector2f(const GLchar* name, glm::vec2 value, GLboolean useShader = false);
  void setVector3f(const GLchar* name, glm::vec3 value, GLboolean useShader = false);
  void setVector4f(const GLchar* name, glm::vec4 value, GLboolean useShader = false);
  void setMatrix4(const GLchar* name, glm::mat4 matrix, GLboolean useShader = false);

private:
  void checkCompileErrors(GLuint object, std::string type);

};

#endif
