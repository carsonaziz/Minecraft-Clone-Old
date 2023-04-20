#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>

#include "utils/yaml_util.h"

#include "glm/glm.hpp"

#define CX 16
#define CY 256
#define CZ 16

class SimplexNoise;

namespace Minecraft
{
    struct Point
    {
        float x;
        float y;
    };

    class Chunk
    {
    private:
        static std::unordered_map<std::string, TextureFormat> texture_map;
        static std::unordered_map<std::string, int> block_map;
        static std::vector<BlockFormat> block_formats;

        // maybe add chunk properties struct
        uint8_t* m_blocks;
        glm::ivec3 m_chunk_position;
        glm::vec3 m_world_position;

        int m_vertex_count;
        bool m_changed;
        unsigned int m_vao, m_vert_vbo, m_norm_vbo, m_uv_vbo;

    private:
        void init();
        void calc_block(const SimplexNoise& generator, int x, int y, int z);
        int spline_height(float perlin_val);
        float spline_squash(float perlin_val);
    public:
        Chunk(const glm::vec3 position);
        ~Chunk();

        int get_block(int x, int y, int z);
        void set_block(int x, int y, int z, int block);

        glm::ivec3 get_chunk_position();
        glm::vec3 get_world_position();
        unsigned int get_vao();
        int get_vertex_count();

        void update();

        static void load_block_formats(std::string filepath);
        static void load_texture_formats(std::string filepath);
    };
}