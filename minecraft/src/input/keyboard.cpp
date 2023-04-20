#include "input/keyboard.h"

#include "GLFW/glfw3.h"

namespace Minecraft
{
    int Keyboard::m_keys[] = { false };

    void Keyboard::update()
    {
    }

    bool Keyboard::is_down(int key)
    {
        return m_keys[key];
    }

    void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (action == GLFW_PRESS)
            m_keys[key] = true;

        if (action == GLFW_RELEASE)
            m_keys[key] = false;
    }
}