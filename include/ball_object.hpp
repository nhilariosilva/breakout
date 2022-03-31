#ifndef BALL_OBJECT_HPP
#define BALL_OBJECT_HPP

#include <game_object.hpp>

class BallObject : public GameObject{
public:
  GLfloat radius;
  GLboolean stuck;

  BallObject();
  BallObject(glm::vec2 pos, GLfloat radius, Texture2D sprite, glm::vec2 velocity);
  ~BallObject();

  glm::vec2 move(GLfloat dt, GLuint windowWidth);
  void reset(glm::vec2 position, glm::vec2 velocity);

};

#endif
