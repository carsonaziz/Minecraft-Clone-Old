#pragma once

#include <string>

#include "glm/glm.hpp"

namespace Minecraft
{
    class Shader
    {
    private:
        unsigned int m_program_id;

    private:
        std::string load_file(const std::string& filepath);
        unsigned int create_shader(const unsigned int type, const std::string& filepath);
    public:
        Shader(const std::string& vertex_path, const std::string& fragment_path);
        ~Shader();

        void load_mat4(const std::string& name, const glm::mat4& mat);

        void use();
    };
}