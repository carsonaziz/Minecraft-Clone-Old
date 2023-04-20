#pragma once

#include <string>
#include <memory>

typedef struct GLFWwindow GLFWwindow;

namespace Minecraft
{
    struct WindowProperties
    {
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

        WindowProperties(const std::string& title, unsigned int width, unsigned int height)
            : m_title(title),
            m_width(width),
            m_height(height) {}
    };

    class Window
    {
    private:
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

        GLFWwindow* p_window;

    private:

        void init(WindowProperties& props);

        static void error_callback(int error, const char* description);
        static void joystick_callback(int jid, int event);
    public:
        Window(WindowProperties& props);
        ~Window();

        void run();
        void poll_and_swap();
        void set_vsync(bool vsync);
        void set_title(std::string title);

        inline unsigned int get_width() const { return m_width; }
        inline unsigned int get_height() const { return m_height; }
        inline GLFWwindow* get_window_ptr() const { return p_window; }

        static std::unique_ptr<Window> create(WindowProperties& props);
    };
}