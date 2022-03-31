#ifndef GAME_HPP
#define GAME_HPP

#include <game_level.hpp>
#include <ball_object.hpp>

#include <tuple>

#include <irrKlang.h>

#include <glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game{
public:
  // Game State
  GameState state;
  GLboolean keys[1024];
  GLuint width, height;
  std::vector<GameLevel> levels;
  GLuint level;

  // Constructor/Destructor
  Game();
  ~Game();

  // Initialize Game State (load textures/shaders/levels)
  void init();

  // Game loop
  void processInput(GLfloat dt);
  void update(GLfloat dt);
  void doCollisions();
  void render();

  void resetLevel();
  void resetPlayer();

private:
  Direction vectorDirection(glm::vec2 target);
  Collision checkCollision(BallObject &one, GameObject &two);
};

#endif
