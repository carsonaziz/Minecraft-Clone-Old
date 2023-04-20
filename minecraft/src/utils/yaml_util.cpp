#include "utils/yaml_util.h"

#include "engine/log.h"

namespace Minecraft
{
    namespace YAMLUTIL
    {
        void write_texture_formats(const std::vector<TextureFormat>& texture_formats)
        {
            YAML::Node texture_format_node;

            for (const auto& texture_format : texture_formats)
            {
                YAML::Node uvs;
                write_vec2("uv0", texture_format.uv0, uvs);
                write_vec2("uv1", texture_format.uv1, uvs);
                write_vec2("uv2", texture_format.uv2, uvs);
                write_vec2("uv3", texture_format.uv3, uvs);

                texture_format_node[texture_format.name] = uvs;
            }

            std::ofstream fout("minecraft/res/blockdata/texture_format.yaml");
            fout << texture_format_node;
            fout.close();
        }

        void read_texture_formats(const std::string filepath, std::unordered_map<std::string, TextureFormat>& texture_map)
        {
            YAML::Node texture_format_node = YAML::LoadFile(filepath);

            for (const auto& texture : texture_format_node)
            {
                YAML::Node texture_uvs = texture.second;

                std::string name = texture.first.as<std::string>();
                glm::vec2 uv0 = read_vec2("uv0", texture_uvs);
                glm::vec2 uv1 = read_vec2("uv1", texture_uvs);
                glm::vec2 uv2 = read_vec2("uv2", texture_uvs);
                glm::vec2 uv3 = read_vec2("uv3", texture_uvs);

                texture_map[name] = { name, uv0, uv1, uv2, uv3 };
            }
        }

        void read_block_formats(const std::string filepath, std::vector<BlockFormat>& block_formats, std::unordered_map<std::string, int>& block_map)
        {
            YAML::Node block_format_node = YAML::LoadFile(filepath);

            for (const auto& block : block_format_node)
            {
                std::string name = block.first.as<std::string>();
                int id = block.second["id"].as<int>();
                std::string side = block.second["side"].as<std::string>();
                std::string top = block.second["top"].as<std::string>();
                std::string bottom = block.second["bottom"].as<std::string>();
                block_formats.push_back({ name, id, side, top, bottom });
                block_map[name] = id;
            }
        }

        void write_vec2(const char* name, const glm::vec2& vec, YAML::Node& node)
        {
            node[name].push_back(vec.x);
            node[name].push_back(vec.y);
        }

        glm::vec2 read_vec2(const char* name, YAML::Node& node)
        {
            return glm::vec2(node[name][0].as<float>(), node[name][1].as<float>());
        }
    }
}