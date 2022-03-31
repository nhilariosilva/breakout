
#include <game_object.hpp>

GameObject::GameObject(){
  this->position = glm::vec2(0, 0);
  this->size = glm::vec2(1, 1);
  this->color = glm::vec3(1.0f);
  this->rotation = 0.0f;
  this->velocity = glm::vec2(0.0f, 0.0f);
  this->isSolid = GL_FALSE;
  this->destroyed = GL_FALSE;
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat rotation, glm::vec3 color){
  this->position = pos;
  this->size = size;
  this->velocity = velocity;
  this->rotation = rotation;
  this->color = color;
  this->sprite = sprite;
  this->isSolid = GL_FALSE;
  this->destroyed = GL_FALSE;
}

GameObject::~GameObject(){ }

void GameObject::draw(SpriteRenderer &renderer){
  renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}
