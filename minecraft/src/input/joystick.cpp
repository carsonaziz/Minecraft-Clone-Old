#include "input/joystick.h"

#include <iostream>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Minecraft
{
    void Joystick::joystick_callback(int jid, int event)
    {
        if (event == GLFW_CONNECTED)
            std::cout << "Connected\n";
        else if (event == GLFW_DISCONNECTED)
            std::cout << "Disconnected\n";
    }
}