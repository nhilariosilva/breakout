#ifndef GAME_LEVEL_HPP
#define GAME_LEVEL_HPP

#include <resource_manager.hpp>
#include <game_object.hpp>

#include <vector>
#include <fstream>
#include <sstream>

class GameLevel{
public:
  std::vector<GameObject> bricks;

  GameLevel();
  ~GameLevel();

  void load(const GLchar* levelFile, GLuint levelWidth, GLuint levelHeight);
  void printTileData(std::vector<std::vector<GLuint>> tileData);
  void draw(SpriteRenderer &renderer);
  GLboolean isCompleted();

private:
  void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif
