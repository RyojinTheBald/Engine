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


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

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

std::vector<float> triangle()
{
  return std::vector<float>{
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f};
}


int main(int argc, char **argv)
{
  tf::Executor executor;  //Executor used for all of our taskflows
  tf::Taskflow prerender; //Prerender task graph
  double dt;              //Inter-frame deltaT

  //Create window
  GWindow window;

  Camera cam = {};




  Assimp::Importer importer;

 const aiScene* scene = importer.ReadFile( "../../assets/cube.dae",
    aiProcess_JoinIdenticalVertices);

  // If the import failed, report it
  if( !scene) {
    std::cerr << "scene import failed: " << importer.GetErrorString() << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Number of meshes in scene: " << scene->mNumMeshes << std::endl;
  std::cout << "Number of materials in scene: " << scene->mNumMaterials << std::endl;

  for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
    auto mesh = scene->mMeshes[i];
    std::cout << "Mesh " << i << std::endl;
    std::cout << "\tName: " << mesh->mName.data << std::endl;
    std::cout << "\tVertices: " << mesh->mNumVertices << std::endl;
    std::cout << "\tFaces: " << mesh->mNumFaces << std::endl;
  }


  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                   "}\0";

  const char *fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\0";

  std::cout << "Compiling shaders" << std::endl;

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Error: Vertex shader compilation failed\n\t" << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Error: Fragment shader compilation failed\n\t" << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }


  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Error: Shader program linking failed\n\t" << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
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
  }).name("Graphics subsystem");

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

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    window.swapBuffers();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
