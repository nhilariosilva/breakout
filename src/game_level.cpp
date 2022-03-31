
#include <game_level.hpp>

GameLevel::GameLevel(){ }

GameLevel::~GameLevel(){ }

void GameLevel::load(const GLchar* levelFile, GLuint levelWidth, GLuint levelHeight){
  this->bricks.clear();

  GLuint tileCode;
  GameLevel level;
  std::string line;
  std::ifstream file(levelFile);
  std::vector<std::vector<GLuint>> tileData;
  if(file){
    while(std::getline(file, line)){
      //istringstream basically runs through a string by its spaces
      std::istringstream iss(line);
      std::vector<GLuint> row;
      while(iss >> tileCode){
        row.push_back(tileCode);
      }
      tileData.push_back(row);
    }
    if(tileData.size() > 0){
      this->init(tileData, levelWidth, levelHeight);
    }
  }

}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight){
  GLuint height = tileData.size();
  GLuint width = tileData[0].size();
  GLfloat unitWidth = (GLfloat) levelWidth / width;
  GLfloat unitHeight = (GLfloat) levelHeight / height;

  for(int i = 0; i < height; i++){
    for(int j = 0; j < width; j++){
      if(tileData[i][j] == 1){
        //Solid
        glm::vec2 pos(unitWidth * j, unitHeight * i);
        glm::vec2 size(unitWidth, unitHeight);
        GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(0.8f, 0.8f, 0.7f));
        obj.isSolid = GL_TRUE;
        this->bricks.push_back(obj);
      }else if(tileData[i][j] > 0){
        //glm::vec3 color = glm::vec3(1.0f);
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
        switch(tileData[i][j]){
          case 2:
            color = glm::vec3(0.2f, 0.6f, 1.0f);
            break;
          case 3:
            color = glm::vec3(0.0f, 0.7f, 0.0f);
            break;
          case 4:
            color = glm::vec3(0.8f, 0.8f, 0.4f);
            break;
          case 5:
            color = glm::vec3(1.0f, 0.5f, 0.0f);
            break;
        }
        glm::vec2 pos(unitWidth * j, unitHeight * i);
        glm::vec2 size(unitWidth, unitHeight);
        GameObject obj(pos, size, ResourceManager::getTexture("block"), glm::vec2(0.0f, 0.0f), 0.0f, color);
        this->bricks.push_back(obj);
      }
    }
  }
}

void GameLevel::printTileData(std::vector<std::vector<GLuint>> tileData){
  std::vector<GLuint> row;
  for(int i = 0; i < tileData.size(); i++){
    row = tileData[i];
    for(int j = 0; j < row.size(); j++){
      std::cout << row[j] << " ";
    }
    std::cout << std::endl;
  }
}

void GameLevel::draw(SpriteRenderer &renderer){
  for(int i = 0; i < this->bricks.size(); i++)
    if(!bricks[i].destroyed)
      bricks[i].draw(renderer);
}

GLboolean GameLevel::isCompleted(){
  for(int i = 0; i < this->bricks.size(); i++)
    if(!bricks[i].isSolid && !bricks[i].destroyed)
      return GL_FALSE;
  return GL_TRUE;
}
