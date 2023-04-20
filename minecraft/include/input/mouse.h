#pragma once

#define GLFW_MOUSE_BUTTON_LAST 7
typedef struct GLFWwindow GLFWwindow;

namespace Minecraft
{
    class Mouse
    {
    private:
        static int m_buttons[GLFW_MOUSE_BUTTON_LAST];
        static int m_current_x, m_current_y;
        static int m_previous_x, m_previous_y;
        static int m_dx, m_dy;
        static int m_xscroll, m_yscroll;

    public:
        static void update();
        
        static int get_dx();
        static int get_dy();

        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    };
}