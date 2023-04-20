#include "graphics/renderer.h"

#include <mutex>

#include "engine/log.h"
#include "world/world.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Minecraft
{
    Renderer::Renderer()
    {
        m_texture_atlas = std::make_unique<TextureAtlas>("minecraft/res/textures/texture_atlas.png");
        m_proj = std::make_unique<glm::mat4>(glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 1000.0f));
        
        init();
    }

    Renderer::~Renderer()
    {

    }

    void Renderer::init()
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::render_world(std::unique_ptr<World>& world, std::unique_ptr<Shader>& shader)
    {
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 model = glm::mat4(1.0f);
        m_view = std::make_unique<glm::mat4>(1.0f);
        
        std::lock_guard<std::mutex> lock(world->get_mutex());
        m_view = std::make_unique<glm::mat4>(glm::lookAt(world->get_player().get_position(), world->get_player().get_position() + world->get_player().get_direction(), glm::vec3(0, 1, 0)));

        shader->load_mat4("model", model);
        shader->load_mat4("view", *m_view);
        shader->load_mat4("proj", *m_proj);

        glBindTexture(GL_TEXTURE0, m_texture_atlas->get());

        for(auto& it : world->get_chunks())
        {
            Chunk* chunk = it.second;
            model = glm::mat4(1.0f);
            model = glm::translate(model, chunk->get_world_position());
            shader->load_mat4("model", model);
            shader->use();
            glBindVertexArray(chunk->get_vao());
            glDrawArrays(GL_TRIANGLES, 0, chunk->get_vertex_count());
        }
    }

    void Renderer::get_gl_error()
    {
        unsigned int error;
        error = glGetError();
        
        while(error != GL_NO_ERROR)
        {
            switch (error)
            {
            case GL_INVALID_ENUM:
                MC_LOG_ERROR("OpenGL: Invalid enum");
                break;
            case GL_INVALID_OPERATION:
                MC_LOG_ERROR("OpenGL: Invalid operation");
                break;
            case GL_INVALID_VALUE:
                MC_LOG_ERROR("OpenGL: Invalid value");
                break;
            case GL_STACK_OVERFLOW:
                MC_LOG_ERROR("OpenGL: Stack overflow");
                break;
            case GL_STACK_UNDERFLOW:
                MC_LOG_ERROR("OpenGL: Stack underflow");
                break;
            case GL_OUT_OF_MEMORY:
                MC_LOG_ERROR("OpenGL: Out of memory");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                MC_LOG_ERROR("OpenGL: Invalid framebuffer operation");
                break;
            case GL_CONTEXT_LOST:
                MC_LOG_ERROR("OpenGL: Context lost");
                break;
            
            default:
                break;
            }
            error = glGetError();
        }
    }
}