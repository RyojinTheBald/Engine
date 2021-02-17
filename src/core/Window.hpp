#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

namespace Core
{
    class Window
    {
    public:
        Window(entt::dispatcher* dispatcher);
        ~Window();

        void events() { glfwPollEvents(); }
        bool shouldClose() { return glfwWindowShouldClose(m_window); }
        void swapBuffers() { glfwSwapBuffers(m_window); }
        void getSize(int *width, int *height) { glfwGetWindowSize(m_window, width, height); }

        operator GLFWwindow*() const { return m_window; }

    protected:
        GLFWwindow* m_window;
        entt::dispatcher* m_dispatcher;

        static void _FramebufferSizeCallback(GLFWwindow *wnd, int x, int y);
        static void _CursorPosCallback(GLFWwindow *wnd, double x, double y);
        static void _MouseButtonCallback(GLFWwindow *wnd, int button, int action, int mods);
        static void _KeyCallback(GLFWwindow *wnd, int key, int scancode, int action, int mods);
        static void _ScrollCallback(GLFWwindow *wnd, double x, double y);
    };
}