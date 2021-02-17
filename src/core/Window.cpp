//Must be included before glfw (in GWindow.hpp)
#include <GL/glew.h>
#include <iostream>

#include "Window.hpp"

#include "../events/all.hpp"

namespace Core
{
  Window::Window(entt::dispatcher *dispatcher) : m_dispatcher(dispatcher)
  {
    //Initialise glfw
    if (!glfwInit())
    {
      std::cerr << "Fatal Error: Unable to initialise glfw3" << std::endl;
      exit(EXIT_FAILURE);
    }

    //Create window and opengl context
    m_window = glfwCreateWindow(640, 480, "OpenGL Test", NULL, NULL);
    if (!m_window)
    {
      glfwTerminate();
      std::cerr << "Fatal Error: Unable to create window" << std::endl;
      exit(EXIT_FAILURE);
    }

    //Activate window's context
    glfwMakeContextCurrent(m_window);

    //Save pointer to window object to use with callbacks
    glfwSetWindowUserPointer(m_window, this);

    //Initialise GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      std::cerr << "Fatal Error: GLEW Initialisation failed" << std::endl;
      exit(EXIT_FAILURE);
    }

    //GLFW Event callbacks
    glfwSetFramebufferSizeCallback(m_window, &Window::_FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, &Window::_CursorPosCallback);
    glfwSetMouseButtonCallback(m_window, &Window::_MouseButtonCallback);
    glfwSetKeyCallback(m_window, &Window::_KeyCallback);
    glfwSetScrollCallback(m_window, &Window::_ScrollCallback);

    //Set up OpenGL

    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // Enable vsync so we don't chew up cpu time
    glfwSwapInterval(1);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    //Back-face culling
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glEnable(GL_LINE_SMOOTH);
    // glLineWidth(3.0f);
    // glPointSize(5);
  }

  Window::~Window()
  {
    glfwTerminate();
  }

  /*
  * GLFW callback functions
  */

  //TODO: this needs to actually affect the projection matrices etc. correctly
  void Window::_FramebufferSizeCallback(GLFWwindow *wnd, int x, int y)
  {
    //Instance reference
    Window *_window = (Window *)glfwGetWindowUserPointer(wnd);

    //std::cout << "Window resized to " << x << "," << y << std::endl;
  }

  void Window::_CursorPosCallback(GLFWwindow *wnd, double x, double y)
  {
    //Instance reference
    Window *_window = (Window *)glfwGetWindowUserPointer(wnd);
    _window->m_dispatcher->trigger<Events::MouseMove>((int)x, (int)y);
  }

  void Window::_MouseButtonCallback(GLFWwindow *wnd, int button, int action, int mods)
  {
    //Instance reference
    Window *_window = (Window *)glfwGetWindowUserPointer(wnd);
    _window->m_dispatcher->trigger<Events::MouseButton>(button, action, mods);
  }

  void Window::_KeyCallback(GLFWwindow *wnd, int key, int scancode, int action, int mods)
  {
    //Instance reference
    Window *_window = (Window *)glfwGetWindowUserPointer(wnd);
    _window->m_dispatcher->trigger<Events::Key>(key, action, mods);
  }

  void Window::_ScrollCallback(GLFWwindow *wnd, double x, double y)
  {
    //Instance reference
    Window *_window = (Window *)glfwGetWindowUserPointer(wnd);
    _window->m_dispatcher->trigger<Events::MouseScroll>((int)x, (int)y);
  }
} // namespace Core