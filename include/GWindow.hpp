#pragma once

#include <GLFW/glfw3.h>


class GWindow {
public:
  GWindow();

  bool shouldClose() { return glfwWindowShouldClose(_window); }
  void swapBuffers() { glfwSwapBuffers(_window); }
  void getSize(int* width, int* height) { glfwGetWindowSize(_window, width, height); }

protected:
  GLFWwindow* _window;


  static void _FramebufferSizeCallback(GLFWwindow* wnd, int x, int y);
  static void _CursorPosCallback(GLFWwindow* wnd, double x, double y);
  static void _MouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods);
  static void _KeyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
  static void _ScrollCallback(GLFWwindow* wnd, double x, double y);
};
