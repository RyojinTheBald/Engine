#define _USE_MATH_DEFINES


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <vector>
#include <cmath>
#include <random>
#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


#include <entt/entt.hpp>

#include "GWindow.hpp"

entt::registry registry;


int main(int argc, char** argv) {
  GWindow window;

  // constructs a naked entity with no components and returns its identifier
  auto entity = registry.create();

  //Main render loop
  while (!window.shouldClose()) {
    static double lastTime = glfwGetTime();
    double thisTime = glfwGetTime();
    double deltaTime = thisTime - lastTime;
    lastTime = thisTime;

    glfwPollEvents();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    window.swapBuffers();
  }


  // destroys an entity and all its components
  registry.destroy(entity);



  glfwTerminate();

  return 0;
}
