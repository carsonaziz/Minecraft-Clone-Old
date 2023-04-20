#include "engine/engine.h"

#include "engine/log.h"
#include "world/world.h"
#include "utils/texture_packer.h"

#include "GLFW/glfw3.h"


namespace Minecraft
{
    Engine& Engine::instance()
    {
        static Engine instance;

        return instance;
    }

    Engine::Engine() : p_window(nullptr), p_renderer(nullptr), m_running(false)
    {
        init();
    }

    Engine::~Engine() 
    {
    }

    void Engine::init()
    {
        Log::init();
        TEXPACK::pack_textures("minecraft/res/textures/blocks/");

        WindowProperties props("Minecraft", 1280, 720);
        p_window = Window::create(props);
        p_window->set_vsync(false);

        p_renderer = std::make_unique<Renderer>();
        p_world_shader = std::make_unique<Shader>("minecraft/res/shaders/vertex.glsl", "minecraft/res/shaders/fragment.glsl");
        p_world = std::make_unique<World>();

        m_running = true;
    }

    void Engine::run()
    {
        double last_time = glfwGetTime();
        int num_frames = 0;
        int num_updates = 0;
        float dt = 0;
        float elapsed_time = 0;
        while (m_running)
        {
            double current_time = glfwGetTime();
            num_frames++;
            dt = current_time - last_time;
            elapsed_time += dt;
            if (elapsed_time >= 1.0)
            {
                // printf("%f ms/frame\n", 1000.0 / double(num_frames));
                std::string title = "Minecraft | FPS: " + std::to_string(num_frames);
                p_window->set_title(title);
                num_frames = 0;
                elapsed_time = 0;
            }
            update(dt);
            render();
            last_time = current_time;
        }
    }

    void Engine::update(const float dt)
    {
        if (glfwWindowShouldClose(p_window->get_window_ptr()))
        {
            m_running = false;
        }

        p_world->update(dt);
    }

    void Engine::render()
    {
        p_renderer->clear();
        p_renderer->render_world(p_world, p_world_shader);
        p_window->poll_and_swap();
    }
}