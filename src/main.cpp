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
#include <taskflow/taskflow.hpp>

#include "GWindow.hpp"

using namespace std::chrono_literals;

entt::registry registry;

//returns dt of calls to this function
double deltaTime() {
  static double lastTime = glfwGetTime();
  double thisTime = glfwGetTime();
  double ret = thisTime - lastTime;
  lastTime = thisTime;
  return ret;
}


int main(int argc, char** argv) {
  tf::Executor executor;    //Executor used for all of our taskflows
  tf::Taskflow prerender;   //Prerender task graph
  double dt;                //Inter-frame deltaT

  //Create window
  GWindow window;

  std::cout << "Constructing task graph" << std::endl;

  prerender.emplace([&](){
    std::cout << "dt: " << dt << std::endl;
  }).name("Framerate Monitor");

  auto t_Input = prerender.emplace([&](tf::Subflow& subflow){}).name("Input subsystem");
  auto t_AI = prerender.emplace([&](){}).name("AI subsystem");

  auto t_Network = prerender.emplace([&](){}).name("Network subsystem");

  auto t_Physics = prerender.emplace([&](){}).name("Physics subsystem");

  auto t_Sound = prerender.emplace([&](){}).name("Sound subsystem");
  auto t_Graphics = prerender.emplace([&](){}).name("Graphics subsystem");


  t_Input.precede(t_Network);
  t_AI.precede(t_Network);

  t_Network.precede(t_Physics);

  t_Physics.precede(t_Sound);
  t_Physics.precede(t_Graphics);





  std::cout << "Entering main loop" << std::endl;

  //switch to true to disable dumping task graph
  bool dumped = false;

  while (!window.shouldClose()) {
    dt = deltaTime();
    glfwPollEvents();

    executor.run(prerender);
    executor.wait_for_all();

    //Dump graph after first render, to capture dynamic tasks
    if (!dumped) {
      prerender.dump(std::cout);
      std::cout << std::flush;
      dumped = true;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //execute single thread render dispatch

    window.swapBuffers();
  }

  glfwTerminate();

  return 0;
}
