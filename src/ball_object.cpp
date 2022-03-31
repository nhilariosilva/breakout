
#include <ball_object.hpp>

BallObject::BallObject() : GameObject(){
  this->radius = 12.5f;
  this->stuck = GL_TRUE;
}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, glm::vec2(2*radius, 2*radius), sprite, velocity, 0.0f, glm::vec3(1.0f)){
  this->radius = radius;
  this->stuck = GL_TRUE;
  this->rotation = 0.0f;
}

BallObject::~BallObject(){ }

glm::vec2 BallObject::move(GLfloat dt, GLuint windowWidth){
  if(!this->stuck){
    this->position += this->velocity * dt;

    if(this->position.x <= 0.0f){
      this->velocity.x = -this->velocity.x;
      this->position.x = 0.0f;
    }else if(this->position.x + this->size.x >= windowWidth){
      this->velocity.x = -this->velocity.x;
      this->position.x = windowWidth - this->size.x;
    }
    if(this->position.y <= 0.0f){
      this->velocity.y = -this->velocity.y;
      this->position.y = 0.0f;
    }
  }

  return this->position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity){
  this->position = position;
  this->velocity = velocity;
  this->stuck = GL_TRUE;
}
