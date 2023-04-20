#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "texture_atlas.h"
#include "shader.h"
#include "world/world.h"

namespace Minecraft
{
    class Renderer
    {
    private:
        std::unique_ptr<TextureAtlas> m_texture_atlas;

        std::unique_ptr<glm::mat4> m_view;
        std::unique_ptr<glm::mat4> m_proj;

    private:
        void get_gl_error();

    public:
        Renderer();
        ~Renderer();

        void init();
        void clear();
        void render_world(std::unique_ptr<World>& world, std::unique_ptr<Shader>& shader);
    };
}