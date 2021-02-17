#define _USE_MATH_DEFINES

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <taskflow/taskflow.hpp>

#include "core/Game.hpp"

using namespace std::chrono_literals;

//returns dt of calls to this function
double deltaTime()
{
  static double lastTime = glfwGetTime();
  double thisTime = glfwGetTime();
  double ret = thisTime - lastTime;
  lastTime = thisTime;
  return ret;
}

// /**
//  * allows binding of movement and mouselook to any entity with 
//  * position and/or rotation components
//  */
// void processInput(GLFWwindow* window, entt::entity controlledEntity)
// {
//   float moveSpeed = 1.f;
//   float mouseSpeed = 0.01f;

//   float roll = 0;

//   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//     glfwSetWindowShouldClose(window, true);

//   if (registry.has<component::position>(controlledEntity))
//   {
//     glm::vec3 direction(0);

//     //keyboard movement
//     if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//       direction.z += 1;
//     if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//       direction.z -= 1;
//     if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//       direction.x += 1;
//     if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//       direction.x -= 1;
//     if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//       direction.y -= 1;
//     if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//       direction.y += 1;
//     if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//       roll += 1;
//     if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//       roll -= 1;


//     if (direction.x != 0 || direction.y != 0 || direction.z != 0)
//     {
//       auto pos = registry.get<component::position>(controlledEntity);

//       if (registry.has<component::rotation>(controlledEntity))
//       {
//         auto rot = registry.get<component::rotation>(controlledEntity);
//         direction = direction * rot;
//       }
//       pos += direction * moveSpeed;
//       registry.replace<component::position>(controlledEntity, pos);
//     }
//   }

//   if (registry.has<component::rotation>(controlledEntity)) 
//     {
//     //mouselook
//     if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
//     {
//       //hide cursor, calculate cursor delta, reset cursor position
//       if(glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
//       {
//         //on initial click, reset cursor position to 0,0 without firing movement callback
//         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//         glfwSetCursorPos(window, 0, 0);
//       }
//       else
//       {
//         double x, y;
//         glfwGetCursorPos(window, &x, &y);
//         glfwSetCursorPos(window, 0, 0);

//         glm::quat oldRotation = registry.get<component::rotation>(controlledEntity);

//         glm::quat qPitch = glm::angleAxis((float)(y * mouseSpeed), glm::vec3(1, 0, 0));
//         glm::quat qYaw = glm::angleAxis((float)(x * mouseSpeed), glm::vec3(0, 1, 0));
//         glm::quat qRoll = glm::angleAxis((float)(roll * mouseSpeed), glm::vec3(0, 0, 1));

//         glm::quat newRotation = glm::normalize(qPitch * qRoll * qYaw) * oldRotation;

//         registry.replace<component::rotation>(controlledEntity, newRotation);
//       }

//     }
//     else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
//     {
//       //unhide cursor
//       glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//     }
//   }

// }

int main(int argc, char **argv)
{
  Core::Game game;
  return game.run();

  /*
  tf::Executor executor;  //Executor used for all of our taskflows
  tf::Taskflow prerender; //Prerender task graph
  double dt;              //Inter-frame deltaT

  std::cout << "Constructing task graph" << std::endl;

  prerender.emplace([&]() {
             std::cout << "fps: " << (1 / dt) << "\r" << std::flush;
           })
      .name("Framerate Monitor");

  auto t_Input = prerender.emplace([&](tf::Subflow &subflow) {

  }).name("Input subsystem");
  auto t_AI = prerender.emplace([&]() {}).name("AI subsystem");

  auto t_Network = prerender.emplace([&]() {}).name("Network subsystem");

  auto t_Physics = prerender.emplace([&]() {}).name("Physics subsystem");

  auto t_Sound = prerender.emplace([&]() {}).name("Sound subsystem");
  auto t_Graphics = prerender.emplace([&]() {
                             })
                        .name("Graphics subsystem");

  t_Input.precede(t_Network);
  t_AI.precede(t_Network);

  t_Network.precede(t_Physics);

  t_Physics.precede(t_Sound);
  t_Physics.precede(t_Graphics);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  std::cout << "Entering main loop" << std::endl;

  //switch to true to disable dumping task graph
  bool dumped = true;

  while (!window.shouldClose())
  {
    dt = deltaTime();
    glfwPollEvents();

    executor.run(prerender);
    executor.wait_for_all();

    // Dump graph after first render, to capture dynamic tasks
    if (!dumped)
    {
      prerender.dump(std::cout);
      std::cout << std::flush;
      dumped = true;
    }
  }
  */
}
