#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <shader.hpp>
#include <texture.hpp>

#include <fstream>
#include <sstream>
#include <map>

class ResourceManager{

public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture2D> Textures;

  static Shader loadShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath, std::string name);
  static Texture2D loadTexture(const GLchar* file, GLboolean alpha, std::string name);

  static Shader getShader(std::string name);
  static Texture2D getTexture(std::string name);

  static void clear();
private:
  ResourceManager();
  ~ResourceManager();

  static Shader loadShaderFromFile(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
  static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);

};

#endif
