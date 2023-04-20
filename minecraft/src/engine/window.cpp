#include "engine/window.h"
#include "engine/log.h"
#include "input/keyboard.h"
#include "input/mouse.h"

#include <iostream>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

// temp
#include "graphics/shader.h"

namespace Minecraft
{
    std::unique_ptr<Window> Window::create(WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }

    Window::Window(WindowProperties& props)
    {
        init(props);
    }

    Window::~Window()
    {
    }

    void Window::init(WindowProperties& props)
    {
        m_title = props.m_title;
        m_width = props.m_width;
        m_height = props.m_height;

        // Set error callback
        glfwSetErrorCallback(error_callback);

        if(!glfwInit())
        {
            MC_LOG_ERROR("Could not initialize GLFW");
            glfwTerminate();
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MINECRAFT_PLATFORM_MAC
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        p_window = glfwCreateWindow((int)m_width, (int)m_height, m_title.c_str(), nullptr, nullptr);
        if (!p_window)
        {
            MC_LOG_ERROR("Could not create GLFW window");
            glfwTerminate();
        }

        // Set keyboard callback
        glfwSetKeyCallback(p_window, Keyboard::key_callback);

        // Set mouse callbacks
        glfwSetCursorPosCallback(p_window, Mouse::cursor_position_callback);
        glfwSetMouseButtonCallback(p_window, Mouse::mouse_button_callback);
        glfwSetScrollCallback(p_window, Mouse::scroll_callback);
        glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set joystick connectin callback
        glfwSetJoystickCallback(joystick_callback);
        
        glfwMakeContextCurrent(p_window);
        MC_LOG_INFO("Initialized window {}, w:{}, h:{}", m_title, m_width, m_height);

#ifdef MINECRAFT_PLATFORM_MAC
        MC_LOG_WARN("Platform: MAC");
#endif
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            MC_LOG_ERROR("Failed to initialzie GLAD");
        }
        glfwSwapInterval(1);


        glClearColor(0.39f, 0.58f, 0.93f, 1);

        MC_LOG_INFO("OpenGL Info: \n\tRenderer: {}\n\tVender: {}\n\tVersion: {}", glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION));
    }

    void Window::poll_and_swap()
    {
        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    void Window::set_vsync(bool vsync)
    {
        if(vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    }

    void Window::set_title(std::string title)
    {
        glfwSetWindowTitle(p_window, title.c_str());
    }

    void Window::error_callback(int error, const char* description)
    {
        MC_LOG_ERROR("Error {}: {}", error, description);
    }

    void Window::joystick_callback(int jid, int event)
    {
        if (event == GLFW_CONNECTED)
        {
            std::cout << "Connected\n";
            std::cout << jid << ": " << glfwGetJoystickName(jid) << '\n';
            int axes_count, button_count;
            const float* axes = glfwGetJoystickAxes(jid, &axes_count);
            const unsigned char* buttons = glfwGetJoystickButtons(jid, &button_count);
            std::cout << "axes: " << axes_count << " buttons: " << button_count << '\n';

            for (int i = 0; i < axes_count; i++)
            {
                std::cout << axes[i] << " ";
            }
            std:: cout << '\n';
            for (int i = 0; i < button_count; i++)
            {
                std::cout << (int)buttons[i] << " ";
            }
            std:: cout << '\n';
        }
        else if (event == GLFW_DISCONNECTED)
        {
            std::cout << "Disconnected\n";
        }
    }
}