#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <texture.hpp>
#include <sprite_renderer.hpp>

class GameObject{
public:
  glm::vec2 position, size, velocity;
  glm::vec3 color;
  GLfloat rotation;
  GLboolean isSolid;
  GLboolean destroyed;

  Texture2D sprite;

  GameObject();
  GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat rotation, glm::vec3 color);
  ~GameObject();

  virtual void draw(SpriteRenderer &renderer);
};

#endif
