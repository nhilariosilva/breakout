#include <game.hpp>
#include <resource_manager.hpp>
#include <sprite_renderer.hpp>

irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(glm::vec2(100.0f, -350.0f));
const GLfloat BALL_RADIUS(12.5f);

Game::Game(){
  this->state = GAME_ACTIVE;
}

Game::~Game(){
  delete Renderer;
}

void Game::init(){
  ResourceManager::loadShader("shaders/vertex_shader", "shaders/fragment_shader", nullptr, "sprite");
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);

  ResourceManager::getShader("sprite").use().setInteger("image", 0);
  ResourceManager::getShader("sprite").setMatrix4("projection", projection);

  Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

  // Load textures
  ResourceManager::loadTexture("resources/textures/background.jpg", GL_FALSE, "background");
  ResourceManager::loadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
  ResourceManager::loadTexture("resources/textures/block.png", GL_FALSE, "block");
  ResourceManager::loadTexture("resources/textures/block_solid.png", GL_FALSE, "block_solid");
  ResourceManager::loadTexture("resources/textures/paddle.png", GL_TRUE, "player");
  // Load levels
  GameLevel one; one.load("resources/levels/01.txt", this->width, this->height * 0.5);
  GameLevel two; two.load("resources/levels/02.txt", this->width, this->height * 0.5);
  GameLevel three; three.load("resources/levels/03.txt", this->width, this->height * 0.5);
  GameLevel four; four.load("resources/levels/04.txt", this->width, this->height * 0.5);

  this->levels.push_back(one);
  this->levels.push_back(two);
  this->levels.push_back(three);
  this->levels.push_back(four);
  this->level = 1;

  glm::vec2 playerPos = glm::vec2(
    this->width / 2 - PLAYER_SIZE.x / 2,
    this->height - PLAYER_SIZE.y
  );

  glm::vec2 ballPos = glm::vec2(
    this->width / 2 - BALL_RADIUS,
    this->height - PLAYER_SIZE.y - 2*BALL_RADIUS
  );

  Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("player"),
  glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(1.0f));

  Ball = new BallObject(ballPos, BALL_RADIUS, ResourceManager::getTexture("face"), INITIAL_BALL_VELOCITY);


  SoundEngine->play2D("resources/audio/breakout.mp3", GL_TRUE);
}

void Game::doCollisions(){
  GameObject box;
  Collision collision;
  for(int i = 0; i < this->levels[this->level-1].bricks.size(); i++){
    box = this->levels[this->level-1].bricks[i];
    if(!box.destroyed){
      collision = checkCollision(*Ball, box);
      if(std::get<0>(collision)){
        if(!box.isSolid){
          this->levels[this->level-1].bricks[i].destroyed = GL_TRUE;
          SoundEngine->play2D("resources/audio/bleep.mp3", GL_FALSE);
        }else{
          SoundEngine->play2D("resources/audio/solid.wav", GL_FALSE);
        }

        Direction dir = std::get<1>(collision);
        glm::vec2 difference = std::get<2>(collision);

        if(dir == LEFT || dir == RIGHT){
          Ball->velocity.x = -Ball->velocity.x;
          GLfloat penetration = Ball->radius - std::abs(difference.x);
          if(dir == LEFT)
            Ball->position.x += penetration;
          else
            Ball->position.x -= penetration;
        }else{
          Ball->velocity.y = -Ball->velocity.y;
          GLfloat penetration = Ball->radius - std::abs(difference.y);
          if(dir == UP)
            Ball->position.y -= penetration;
          else
            Ball->position.y += penetration;
        }
      }
    }
  }

  collision = checkCollision(*Ball, *Player);
  if (!Ball->stuck && std::get<0>(collision)){
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = Player->position.x + Player->size.x / 2;
        GLfloat distance = (Ball->position.x + Ball->radius) - centerBoard;
        GLfloat percentage = distance / (Player->size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->velocity;
        Ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->velocity.y = -1 *std::abs(Ball->velocity.y);
        Ball->velocity = glm::normalize(Ball->velocity) * glm::length(oldVelocity);

        SoundEngine->play2D("resources/audio/bleep.wav", GL_FALSE);
    }
}

void Game::processInput(GLfloat dt){
  if(this->state == GAME_ACTIVE){
    GLfloat velocity = PLAYER_VELOCITY * dt;
    if(this->keys[GLFW_KEY_A]){
      if(Player->position.x >= 0){
        Player->position.x -= velocity;
        if(Ball->stuck)
          Ball->position.x -= velocity;
      }
    }
    if(this->keys[GLFW_KEY_D]){
      if(Player->position.x + Player->size.x <= this->width){
        Player->position.x += velocity;
        if(Ball->stuck)
          Ball->position.x += velocity;
      }
    }
    if(this->keys[GLFW_KEY_SPACE]){
      if(Ball->stuck)
        Ball->stuck = GL_FALSE;
    }
  }
}

void Game::update(GLfloat dt){
  Ball->move(dt, this->width);
  this->doCollisions();

  if(Ball->position.y >= this->height){
    this->resetLevel();
    this->resetPlayer();
  }

  if(this->levels[level-1].isCompleted() && level < 4){
    level++;
    this->resetLevel();
    this->resetPlayer();
  }
}

void Game::render(){
  if(this->state == GAME_ACTIVE){
    // Draw background
    Renderer->drawSprite(ResourceManager::getTexture("background"),
    glm::vec2(0, 0), glm::vec2(this->width, this->height));
    // Draw level
    this->levels[this->level-1].draw(*Renderer);
    // Draw player
    Player->draw(*Renderer);
    Ball->draw(*Renderer);
  }
}

Direction Game::vectorDirection(glm::vec2 target){
  glm::vec2 compass[] = {
    glm::vec2(0.0f, 1.0f),  // UP
    glm::vec2(1.0f, 0.0f),  // RIGHT
    glm::vec2(0.0f, -1.0f), // DOWN
    glm::vec2(-1.0f, 0.0f)  // LEFT
  };
  GLfloat max = -1.0f;
  GLuint maxPos = -1;
  for(int i = 0; i < 4; i++){
    GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
    if(dotProduct > max){
      max = dotProduct;
      maxPos = i;
    }
  }
  return (Direction)maxPos;
}

Collision Game::checkCollision(BallObject &one, GameObject &two){
  glm::vec2 center = glm::vec2(one.position.x + one.size.x/2, one.position.y + one.size.y/2);
  glm::vec2 A = glm::vec2(two.position.x, two.position.y);
  glm::vec2 B = glm::vec2(two.position.x + two.size.x, two.position.y);
  glm::vec2 C = glm::vec2(two.position.x + two.size.x, two.position.y + two.size.y);
  glm::vec2 D = glm::vec2(two.position.x, two.position.y + two.size.y);
  glm::vec2 closest = glm::vec2(0.0f);

  GLboolean centerBetweenX = center.x >= A.x && center.x <= B.x;
  GLboolean centerBetweenY = center.y >= A.y && center.y <= D.y;

  if(centerBetweenX){
      if(center.y < A.y)
        closest = glm::vec2(center.x, A.y);
      else if(center.y > D.y)
        closest = glm::vec2(center.x, D.y);
  }else if(centerBetweenY){
      if(center.x < A.x)
        closest = glm::vec2(A.x, center.y);
      else if(center.x > B.x)
        closest = glm::vec2(B.x, center.y);
  }else{
    if(center.x < A.x && center.y < A.y)
      closest = glm::vec2(A.x, A.y);
    else if(center.x > B.x && center.y < B.y)
      closest = glm::vec2(B.x, B.y);
    else if(center.x > C.x && center.y > C.y)
      closest = glm::vec2(C.x, C.y);
    else if(center.x < D.x && center.y > D.y)
      closest = glm::vec2(D.x, D.y);
  }

  glm::vec2 difference = closest - center;
  if( glm::length(difference) < one.radius)
    return std::make_tuple(GL_TRUE, this->vectorDirection(difference), difference);

  return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f ,0.0f));
}

void Game::resetLevel(){
    if (this->level == 1)
      this->levels[0].load("resources/levels/01.txt", this->width, this->height * 0.5f);
    else if (this->level == 2)
        this->levels[1].load("resources/levels/02.txt", this->width, this->height * 0.5f);
    else if (this->level == 3)
        this->levels[2].load("resources/levels/03.txt", this->width, this->height * 0.5f);
    else if (this->level == 4)
        this->levels[3].load("resources/levels/04.txt", this->width, this->height * 0.5f);
}

void Game::resetPlayer(){
    // Reset player/ball stats
    Player->size = PLAYER_SIZE;
    Player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Ball->reset(Player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}
