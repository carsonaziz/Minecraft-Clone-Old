#pragma once

#include <string>

#include "glm/glm.hpp"

namespace Minecraft
{
    struct TextureFormat
    {
        std::string name;
        glm::vec2 uv0;
        glm::vec2 uv1;
        glm::vec2 uv2;
        glm::vec2 uv3;
    };

    struct BlockFormat
    {
        std::string name;
        int id;
        std::string side;
        std::string top;
        std::string bottom;
    };
}