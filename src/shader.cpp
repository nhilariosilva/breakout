
#include <shader.hpp>

Shader::Shader(){ }

Shader::~Shader(){ }

Shader &Shader::use(){
  glUseProgram(this->ID);
  return *this;
}

void Shader::compile(const GLchar* vertexCode, const GLchar* fragmentCode, const GLchar* geometryCode){
  GLuint vShader, fShader, gShader;

  // Vertex Shader
  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vertexCode, NULL);
  glCompileShader(vShader);
  checkCompileErrors(vShader, "VERTEX");

  // Fragment Shader
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fragmentCode, NULL);
  glCompileShader(fShader);
  checkCompileErrors(fShader, "FRAGMENT");

  // Geometry Shader
  if(geometryCode != nullptr){
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gShader, 1, &geometryCode, NULL);
    glCompileShader(gShader);
    checkCompileErrors(gShader, "GEOMETRY");
  }

  this->ID = glCreateProgram();
  glAttachShader(this->ID, vShader);
  glAttachShader(this->ID, fShader);
  if(geometryCode != nullptr)
    glAttachShader(this->ID, gShader);

  glLinkProgram(this->ID);
  checkCompileErrors(this->ID, "PROGRAM");

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  if(geometryCode != nullptr)
    glDeleteShader(gShader);
}

void Shader::setInteger(const GLchar* name, GLuint value, GLboolean useShader){
  if (useShader)
    this->use();
  glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setFloat(const GLchar* name, GLfloat value, GLboolean useShader){
  if (useShader)
    this->use();
  glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::setVector2f(const GLchar* name, glm::vec2 value, GLboolean useShader){
  if (useShader)
    this->use();
  glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar* name, glm::vec3 value, GLboolean useShader){
  if (useShader)
    this->use();
  glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar* name, glm::vec4 value, GLboolean useShader){
  if (useShader)
    this->use();
  glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar* name, glm::mat4 matrix, GLboolean useShader){
  if (useShader)
    this->use();
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type){
  GLint success;
  GLchar infoLog[1024];
    if (type != "PROGRAM"){
      glGetShaderiv(object, GL_COMPILE_STATUS, &success);
      if (!success){
        glGetShaderInfoLog(object, 1024, NULL, infoLog);
        std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << std::endl
                  << infoLog << "\n --------------------------------------------------------- "
                  << std::endl;
      }
    }else{
      glGetProgramiv(object, GL_LINK_STATUS, &success);
      if (!success){
        glGetProgramInfoLog(object, 1024, NULL, infoLog);
        std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << std::endl
                  << infoLog << "\n --------------------------------------------------------- "
                  << std::endl;
      }
    }
}
