
#include <game.hpp>
#include <resource_manager.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
const GLchar* TITLE = "Breakout";

Game Breakout = Game();

void initWindow(GLFWwindow** window, GLuint* width, GLuint* height, const GLchar* title);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char* args[]){
  GLFWwindow* window;

  GLuint screen_width;
  GLuint screen_height;

  initWindow(&window, &screen_width, &screen_height, TITLE);

  glfwSetKeyCallback(window, key_callback);

  // OpenGL configurations
  glViewport(0, 0, screen_width, screen_height);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwFocusWindow(window);

  // Initialize the game
  Breakout.width = screen_width;
  Breakout.height = screen_height;
  Breakout.init();

  GLfloat timeElapsed = 0.0f;
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  while(!glfwWindowShouldClose(window)){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();

    timeElapsed = glfwGetTime();
    if(lastFrame == 0){
      lastFrame = timeElapsed;
    }else{
      deltaTime = timeElapsed - lastFrame;
      lastFrame = timeElapsed;
    }

    Breakout.processInput(deltaTime);

    Breakout.update(deltaTime);

    Breakout.render();
    glfwSwapBuffers(window);
  }

  return 0;
}

void initWindow(GLFWwindow** window, GLuint* width, GLuint* height, const GLchar* title){
  glfwInit();

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  *window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);

  *width = mode->width;
  *height = mode->height;

  if( (*window) == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
  }

  glfwMakeContextCurrent(*window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024){
    if (action == GLFW_PRESS){
      Breakout.keys[key] = GL_TRUE;
    }else if (action == GLFW_RELEASE)
      Breakout.keys[key] = GL_FALSE;
  }
}
