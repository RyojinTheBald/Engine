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
#include "Camera.hpp"
#include "Shader.hpp"

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

using namespace std::chrono_literals;

entt::registry registry;

//returns dt of calls to this function
double deltaTime()
{
  static double lastTime = glfwGetTime();
  double thisTime = glfwGetTime();
  double ret = thisTime - lastTime;
  lastTime = thisTime;
  return ret;
}


int main(int argc, char **argv)
{
  tf::Executor executor;  //Executor used for all of our taskflows
  tf::Taskflow prerender; //Prerender task graph
  double dt;              //Inter-frame deltaT

  GWindow window;
  Camera cam;

  Shader shader;
  shader.addFile("../../assets/shaders/vertex.glsl", ShaderType::SHADER_VERTEX);
  shader.addFile("../../assets/shaders/fragment.glsl", ShaderType::SHADER_FRAGMENT);
  shader.link();

  if (!shader.isValid())
  {
    std::cerr << "Error linking shader" << std::endl;
    exit(EXIT_FAILURE);
  }

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile("../../assets/cube.dae",
                                           aiProcess_JoinIdenticalVertices);

  // If the import failed, report it
  if (!scene)
  {
    std::cerr << "scene import failed: " << importer.GetErrorString() << std::endl;
    exit(EXIT_FAILURE);
  }

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  //generate and upload geometry

  std::vector<float> vertices = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f};

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "Constructing task graph" << std::endl;

  prerender.emplace([&]() {
             std::cout << "fps: " << (1 / dt) << "\r" << std::flush;
           })
      .name("Framerate Monitor");

  auto t_Input = prerender.emplace([&](tf::Subflow &subflow) {}).name("Input subsystem");
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
  bool dumped = false;

  while (!window.shouldClose())
  {
    dt = deltaTime();
    glfwPollEvents();

    executor.run(prerender);
    executor.wait_for_all();

    //Dump graph after first render, to capture dynamic tasks
    // if (!dumped)
    // {
    //   prerender.dump(std::cout);
    //   std::cout << std::flush;
    //   dumped = true;
    // }

    glClear(GL_COLOR_BUFFER_BIT);

    //execute single thread render dispatch

    glUseProgram(shader.getProgram());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    window.swapBuffers();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
