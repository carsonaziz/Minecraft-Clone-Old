#include "graphics/shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "engine/log.h"
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE

namespace Minecraft
{
    Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
    {
        unsigned int vertex_id = create_shader(GL_VERTEX_SHADER, vertex_path);
        unsigned int fragment_id = create_shader(GL_FRAGMENT_SHADER, fragment_path);

        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, vertex_id);
        MC_LOG_INFO("SHADER::Attached vertex shader");
        glAttachShader(m_program_id, fragment_id);
        MC_LOG_INFO("SHADER::Attached fragment shader");
        glLinkProgram(m_program_id);

        int  success;
        char infoLog[512];
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
        if(!success) 
        {
            glGetProgramInfoLog(m_program_id, 512, NULL, infoLog);
            MC_LOG_ERROR("SHADER::Shader program linking failed: {}", infoLog);
        }
        else
        {
            MC_LOG_INFO("SHADER::Linked shader program");
        }

        glDeleteShader(vertex_id);
        glDeleteShader(fragment_id);
    }

    Shader::~Shader()
    {

    }

    void Shader::load_mat4(const std::string& name, const glm::mat4& mat)
    {
        glUseProgram(m_program_id);
        unsigned int location = glGetUniformLocation(m_program_id, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }

    std::string Shader::load_file(const std::string& filepath)
    {
        std::string code;
        std::ifstream file;

        try
        {
            file.open(filepath);
            std::stringstream filestream;

            filestream << file.rdbuf();

            file.close();

            code = filestream.str();
        }
        catch(std::ifstream::failure e)
        {
            MC_LOG_ERROR("SHADER::File not succesfully read");
        }

        return code;
    }

    unsigned int Shader::create_shader(const unsigned int type, const std::string& filepath)
    {
        unsigned int shader_id = glCreateShader(type);
        std::string shader_str = load_file(filepath);
        const char* shader_source = shader_str.c_str();
        glShaderSource(shader_id, 1, &shader_source, NULL);
        glCompileShader(shader_id);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            if (type == GL_VERTEX_SHADER)
            {
                MC_LOG_ERROR("SHADER::Vertex shader compilation failed: {}", infoLog);
            }
            else if (type == GL_FRAGMENT_SHADER)
            {
                MC_LOG_ERROR("SHADER::Fragment shader compilation failed: {}", infoLog);
            }
        }
        else
        {
            if (type == GL_VERTEX_SHADER)
            {
                MC_LOG_INFO("SHADER::Vertex shader compiled");
            }
            else if (type == GL_FRAGMENT_SHADER)
            {
                MC_LOG_INFO("SHADER::Fragment shader compiled");
            }
        }

        return shader_id;
    } 

    void Shader::use()
    {
        glUseProgram(m_program_id);
    }
}