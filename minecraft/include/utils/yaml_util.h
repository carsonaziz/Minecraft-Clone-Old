#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "config/config_format.h"

namespace Minecraft
{
    namespace YAMLUTIL
    {
        void write_texture_formats(const std::vector<TextureFormat>& texture_formats);
        void read_texture_formats(const std::string filepath, std::unordered_map<std::string, TextureFormat>& texture_map);
        void read_block_formats(const std::string filepath, std::vector<BlockFormat>& block_formats, std::unordered_map<std::string, int>& block_map);

        void write_vec2(const char* name, const glm::vec2& vec, YAML::Node& node);
        glm::vec2 read_vec2(const char* name, YAML::Node& node);
    }
}