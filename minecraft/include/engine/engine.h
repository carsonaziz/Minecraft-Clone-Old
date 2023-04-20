#pragma once

#include <memory>

#include "engine/window.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "world/world.h"

#define TARGET_UPS 60

namespace Minecraft
{
    class Engine
    {
    private:
        // might need to change to shared_ptr
        std::unique_ptr<Window> p_window;
        std::unique_ptr<Renderer> p_renderer;
        std::unique_ptr<Shader> p_world_shader;
        std::unique_ptr<World> p_world;

        bool m_running;

    private:
        void init();
        void update(const float dt);
        void render();
    public:
        Engine();
        ~Engine();

        void run();
        void get_renderer();

        static Engine& instance();
    };
}