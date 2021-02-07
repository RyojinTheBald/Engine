#define _USE_MATH_DEFINES

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <vector>
#include <cmath>
#include <random>
#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <entt/entt.hpp>
#include <taskflow/taskflow.hpp>

#include "Registry.hpp"
#include "GWindow.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

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

/**
 * allows binding of movement and mouselook to any entity with 
 * position and/or rotation components
 */
void processInput(GLFWwindow* window, entt::entity controlledEntity)
{
  float moveSpeed = 1.f;
  float mouseSpeed = 0.01f;

  float roll = 0;

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (registry.has<component::position>(controlledEntity))
  {
    glm::vec3 direction(0);

    //keyboard movement
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      direction.z += 1;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      direction.z -= 1;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      direction.x += 1;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      direction.x -= 1;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      direction.y -= 1;
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
      direction.y += 1;
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
      roll += 1;
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      roll -= 1;


    if (direction.x != 0 || direction.y != 0 || direction.z != 0)
    {
      auto pos = registry.get<component::position>(controlledEntity);

      if (registry.has<component::rotation>(controlledEntity))
      {
        auto rot = registry.get<component::rotation>(controlledEntity);
        direction = direction * rot;
      }
      pos += direction * moveSpeed;
      registry.replace<component::position>(controlledEntity, pos);
    }
  }

  if (registry.has<component::rotation>(controlledEntity)) 
    {
    //mouselook
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
      //hide cursor, calculate cursor delta, reset cursor position
      if(glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
      {
        //on initial click, reset cursor position to 0,0 without firing movement callback
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, 0, 0);
      }
      else
      {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glfwSetCursorPos(window, 0, 0);

        glm::quat oldRotation = registry.get<component::rotation>(controlledEntity);

        glm::quat qPitch = glm::angleAxis((float)(y * mouseSpeed), glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis((float)(x * mouseSpeed), glm::vec3(0, 1, 0));
        glm::quat qRoll = glm::angleAxis((float)(roll * mouseSpeed), glm::vec3(0, 0, 1));

        glm::quat newRotation = glm::normalize(qPitch * qRoll * qYaw) * oldRotation;

        registry.replace<component::rotation>(controlledEntity, newRotation);
      }

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
    {
      //unhide cursor
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

}

int main(int argc, char **argv)
{
  tf::Executor executor;  //Executor used for all of our taskflows
  tf::Taskflow prerender; //Prerender task graph
  double dt;              //Inter-frame deltaT

  GWindow window;
  auto e_cam = component::camera::create();

  Shader shader;
  shader.addFile("../../assets/shaders/vertex3Dcamera.glsl", ShaderType::SHADER_VERTEX);
  shader.addFile("../../assets/shaders/fragment.glsl", ShaderType::SHADER_FRAGMENT);
  shader.link();

  if (!shader.isValid())
  {
    std::cerr << "Error linking shader" << std::endl;
    exit(EXIT_FAILURE);
  }

  Assimp::Importer importer;

  // const aiScene *scene = importer.ReadFile("../../assets/cube.dae", 0);
  const aiScene *scene = importer.ReadFile("../../assets/cabin.dae", 0);

  // If the import failed, report it
  if (!scene)
  {
    std::cerr << "scene import failed: " << importer.GetErrorString() << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!scene->HasMeshes())
  {
    std::cerr << "Error: no meshes in imported scene" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (scene->HasCameras())
  {
    std::cout << "scene has camera, loading configuration" << std::endl;
    //only pay attention to first camera
    auto sceneCamera = scene->mCameras[0];

    //scene nodes are distinct from entities
    auto rootNode = scene->mRootNode;
    auto cameraNode = rootNode->FindNode(sceneCamera->mName);

    //ascend from camera to root node, and accumulate transformations
    aiMatrix4x4 transform;
    while (cameraNode != rootNode) {
      transform = transform * cameraNode->mTransformation;
      cameraNode = cameraNode->mParent;
    }

    //extract rotation from transformation matrix
    aiMatrix4x4 rotation = transform;
    rotation.a4 = rotation.b4 = rotation.c4 = 0.f;
    rotation.d4 = 1.f;

    auto position = (transform * sceneCamera->mPosition) * 15.0f; //scaling factor for reasons
    auto lookAt = rotation * sceneCamera->mLookAt;
    //at present, camera is locked to horiziontal, so the stored "up" isn't used
    // auto up = rotation * sceneCamera->mUp;


     glm::quat rot = glm::quat_cast(glm::lookAt(glm::vec3(0), glm::vec3(lookAt[0], lookAt[1],lookAt[2]), glm::vec3(0,-1,0)));

    registry.replace<component::position>(e_cam, glm::vec3(position[0], position[1], position[2]));
    registry.replace<component::rotation>(e_cam, rot);

    // cam = {
    //   {position[0], position[1], position[2]}, 
    //   {0,0,0},
    //   glm::degrees(sceneCamera->mHorizontalFOV),
    //   // 90,
    //   sceneCamera->mAspect,
    //   sceneCamera->mClipPlaneNear,
    //   sceneCamera->mClipPlaneFar
    // };

    // cam.lookAt({lookAt[0], lookAt[1], lookAt[2]});

    std::cout << "Initial camera position: " << position[0] << "," << position[1] << "," << position[2] << std::endl;
    std::cout << "Initial camera target: " << lookAt[0] << "," << lookAt[1] << "," << lookAt[2] << std::endl;
    std::cout << "Initial fov (degrees): " << glm::degrees(sceneCamera->mHorizontalFOV) << std::endl;
    std::cout << "Initial aspect ratio: " << sceneCamera->mAspect << std::endl;
  }


  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  //generate and upload geometry

  int numVertices = scene->mMeshes[0]->mNumVertices;
  std::cout << numVertices << " vertices in mesh" << std::endl;

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), scene->mMeshes[0]->mVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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

    processInput(window.getWindow(), e_cam);

    //execute single thread render dispatch
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader.getProgram());

    //update camera matrices
    auto projection = component::camera::getProjection(e_cam);
    glUniformMatrix4fv(shader.getUniformLocation("u_projectionMat44"), 1, GL_FALSE, glm::value_ptr(projection));

    auto view = component::camera::getView(e_cam);
    glUniformMatrix4fv(shader.getUniformLocation("u_viewMat44"), 1, GL_FALSE, glm::value_ptr(view));


    //TODO: get model matrices from scenegraph, also, create a scenegraph lel
    glm::mat4 modelMatrix = glm::mat4(1.0);
    glUniformMatrix4fv(shader.getUniformLocation("u_modelMat44"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    window.swapBuffers();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}
