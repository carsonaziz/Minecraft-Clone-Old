#include "input/mouse.h"
#include <iostream>

#include "GLFW/glfw3.h"

namespace Minecraft
{
    int Mouse::m_buttons[] = { false };
    int Mouse::m_current_x = 0;
    int Mouse::m_current_y = 0;
    int Mouse::m_previous_x = 0;
    int Mouse::m_previous_y = 0;
    int Mouse::m_dx = 0;
    int Mouse::m_dy = 0;
    int Mouse::m_xscroll = 0;
    int Mouse::m_yscroll = 0;

    void Mouse::update()
    {
        m_dx = m_current_x - m_previous_x;
        m_dy = m_current_y - m_previous_y;

        m_previous_x = m_current_x;
        m_previous_y = m_current_y;
    }

    int Mouse::get_dx()
    {
        return Mouse::m_dx;
    }

    int Mouse::get_dy()
    {
        return Mouse::m_dy;
    }

    void Mouse::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        Mouse::m_current_x = xpos;
        Mouse::m_current_y = ypos;
    }

    void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
            Mouse::m_buttons[button] = true;
        
        if (action == GLFW_RELEASE)
            Mouse::m_buttons[button] = false;
    }

    void Mouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Mouse::m_xscroll = xoffset;
        Mouse::m_yscroll = yoffset;
    }
}