//Must be included before glfw (in GWindow.hpp)
#include <GL/glew.h>

#include "GWindow.hpp"

#include <iostream>

GWindow::GWindow() {
  //Initialise glfw
  if (!glfwInit()){
    std::cerr << "Fatal Error: Unable to initialise glfw3" << std::endl;
    exit(EXIT_FAILURE);
  }

  //Create window and opengl context
  _window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
  if (!_window)
  {
      glfwTerminate();
      std::cerr << "Fatal Error: Unable to create window" << std::endl;
      exit(EXIT_FAILURE);
  }

  //Activate window's context
  glfwMakeContextCurrent(_window);

  //Save pointer to window object to use with callbacks
  glfwSetWindowUserPointer(_window, this);

  //Initialise GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Fatal Error: GLEW Initialisation failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  //GLFW Event callbacks
  glfwSetFramebufferSizeCallback(_window, &GWindow::_FramebufferSizeCallback);
  glfwSetCursorPosCallback(_window, &GWindow::_CursorPosCallback);
  glfwSetMouseButtonCallback(_window, &GWindow::_MouseButtonCallback);
  glfwSetKeyCallback(_window, &GWindow::_KeyCallback);
  glfwSetScrollCallback(_window, &GWindow::_ScrollCallback);


  //Set up OpenGL

	const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

  // Enable vsync so we don't chew up cpu time
	glfwSwapInterval(1);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Enable depth test
	// glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


  //Back-face culling
	//glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);


	glEnable(GL_LINE_SMOOTH);
  // glLineWidth(3.0f);
	// glPointSize(5);

}









/*
 * GLFW callback functions
 */

void GWindow::_FramebufferSizeCallback(GLFWwindow* wnd, int x, int y) {
  //Get reference to owning GWindow instance
  GWindow* _window = (GWindow*)glfwGetWindowUserPointer(wnd);

  //std::cout << "Window resized to " << x << "," << y << std::endl;
}


void GWindow::_CursorPosCallback(GLFWwindow* wnd, double x, double y) {
  //Get reference to owning GWindow instance
  GWindow* _window = (GWindow*)glfwGetWindowUserPointer(wnd);

  //std::cout << "Cursor moved to " << x << "," << y << std::endl;
}

void GWindow::_MouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods) {
  //Get reference to owning GWindow instance
  GWindow* _window = (GWindow*)glfwGetWindowUserPointer(wnd);

  //std::cout << "Mouse button " << button << "," << action << "," << mods << std::endl;
}

void GWindow::_KeyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
  //Get reference to owning GWindow instance
  GWindow* _window = (GWindow*)glfwGetWindowUserPointer(wnd);

  //std::cout << "Key event " << key << "," << scancode << "," << action << "," << mods << std::endl;
}

void GWindow::_ScrollCallback(GLFWwindow* wnd, double x, double y) {
  //Get reference to owning GWindow instance
  GWindow* _window = (GWindow*)glfwGetWindowUserPointer(wnd);

  //std::cout << "Scroll event " << x << "," << y << std::endl;
}
