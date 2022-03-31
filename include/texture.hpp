#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture2D{
public:
  GLuint ID;
  GLuint width, height;

  // Texture format
  GLuint internalFormat;
  GLuint imageFormat;

  // Texture configuration
  GLuint wrapS;
  GLuint wrapT;
  GLuint filterMin;
  GLuint filterMax;

  Texture2D();
  ~Texture2D();

  // Generates texture from image data
  void generate(GLuint width, GLuint height, unsigned char* data);

  // Binds the texture as the current active GL_TEXTURE_2D texture object
  void bind() const;
  // the const means the method won't attempt to change the object
};

#endif
