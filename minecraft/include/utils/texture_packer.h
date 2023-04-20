#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Minecraft
{
    namespace TEXPACK
    {
        void pack_textures(const char* dir);
        unsigned char* load_texture(std::string filepath, int& width, int& height, int& num_channels);
    }
}