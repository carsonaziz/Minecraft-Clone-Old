#pragma once

#define GLFW_KEY_LAST 348
typedef struct GLFWwindow GLFWwindow;

namespace Minecraft
{
    class Keyboard
    {
    private:
        static int m_keys[GLFW_KEY_LAST];

    public:
        static void update();

        static bool is_down(int key);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}