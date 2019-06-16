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


entt::registry registry;


GLFWwindow* createWindow() {
  if (!glfwInit()){
    std::cerr << "Fatal Error: Unable to initialise glfw3" << std::endl;
    exit(-1);
  }


  glfwWindowHint(GLFW_SAMPLES, 8);

  /* Create a windowed mode window and its OpenGL context */
  auto glfwWindow = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
  if (!glfwWindow)
  {
      glfwTerminate();
      std::cerr << "Fatal Error: Unable to create window" << std::endl;
      exit(-1);
  }
  /* Make the window's context current */
  glfwMakeContextCurrent(glfwWindow);

  //Initialise GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Fatal Error: GLEW Initialisation failed" << std::endl;
    exit(-1);
  }

  // needed for glfwGetUserPointer to work
  //glfwSetWindowUserPointer(glfwWindow, this);

  // set our static functions as callbacks
  //glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);
  //glfwSetCursorPosCallback(glfwWindow, CursorPosCallback);
  //glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
  //glfwSetKeyCallback(glfwWindow, KeyCallback);

  // Initializes projection matrix
  int width, height;
  glfwGetWindowSize(glfwWindow, &width, &height);
  //framebufferResize(width, height);
  return glfwWindow;
}


void setupOpenGL(){
	const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

    // Enable vsync so we don't chew up cpu time
	glfwSwapInterval(1);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3.0f);

	glPointSize(5);
}


int main(int argc, char** argv) {

  GLFWwindow* window = createWindow();

  setupOpenGL();

  // constructs a naked entity with no components and returns its identifier
  auto entity = registry.create();

  //Main render loop
  while (!glfwWindowShouldClose(window)) {
    static double lastTime = glfwGetTime();
    double thisTime = glfwGetTime();
    double deltaTime = thisTime - lastTime;
    lastTime = thisTime;

    glfwPollEvents();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glfwSwapBuffers(window);
  }


  // destroys an entity and all its components
  registry.destroy(entity);



  glfwTerminate();

  return 0;
}
