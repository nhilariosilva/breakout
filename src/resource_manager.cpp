
#include <resource_manager.hpp>

#include <stb_image.h>

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

ResourceManager::ResourceManager(){ }

ResourceManager::~ResourceManager(){ }

Shader ResourceManager::loadShader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath, std::string name){
  Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath, geometryPath);
  return Shaders[name];
}

Texture2D ResourceManager::loadTexture(const GLchar* file, GLboolean alpha, std::string name){
  Textures[name] = loadTextureFromFile(file, alpha);
  return Textures[name];
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath){
  std::ifstream vertexFile, fragmentFile, geometryFile;
  std::stringstream vertexStream, fragmentStream, geometryStream;

  vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  geometryFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try{
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);

    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    if(geometryPath != nullptr){
      geometryFile.open(geometryPath);
      geometryStream << geometryFile.rdbuf();

      geometryFile.close();
    }

    vertexFile.close();
    fragmentFile.close();
  }catch(std::ifstream::failure e){
    std::cout << "ERROR: It was not possible to read shader files" << std::endl;
  }

  std::string vertexCodeStr = vertexStream.str();
  std::string fragmentCodeStr = fragmentStream.str();
  std::string geometryCodeStr = "";

  const char* vertexCode = vertexCodeStr.c_str();
  const char* fragmentCode = fragmentCodeStr.c_str();
  const char* geometryCode = nullptr;

  if(geometryPath != nullptr){
    geometryCodeStr = geometryStream.str();
    geometryCode = geometryCodeStr.c_str();
  }

  Shader shader;
  shader.compile(vertexCode, fragmentCode, geometryCode != nullptr ? geometryCode : nullptr);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha){
  Texture2D texture;
  if(alpha){
    texture.internalFormat = GL_RGBA;
    texture.imageFormat = GL_RGBA;
  }

  int width, height, numChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(file, &width, &height, &numChannels, 0);

  texture.generate(width, height, data);
  stbi_image_free(data);
  return texture;
}

Shader ResourceManager::getShader(std::string name){
  return Shaders[name];
}

Texture2D ResourceManager::getTexture(std::string name){
  return Textures[name];
}

void ResourceManager::clear(){
  for (auto iter : Shaders)
    glDeleteProgram(iter.second.ID);
  for (auto iter : Textures)
    glDeleteTextures(1, &iter.second.ID);
}
