#include "world/chunk.h"

#include "engine/log.h"
#include "simplexnoise/SimplexNoise.h"

#include "glm/glm.hpp"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE

namespace Minecraft
{
    std::unordered_map<std::string, TextureFormat> Chunk::texture_map = std::unordered_map<std::string, TextureFormat>();
    std::unordered_map<std::string, int> Chunk::block_map = std::unordered_map<std::string, int>();
    std::vector<BlockFormat> Chunk::block_formats = std::vector<BlockFormat>();

    Chunk::Chunk(const glm::vec3 position) : m_chunk_position(position), m_world_position(position * 16.0f), m_changed(true)
    {   
        init();
        
        MC_LOG_INFO("Chunk created at: {}, {}, {}", m_chunk_position.x, m_chunk_position.y, m_chunk_position.z);
    }

    Chunk::~Chunk()
    {
        MC_LOG_INFO("Chunk deleted");
        glDeleteBuffers(1, &m_vert_vbo);
        glDeleteBuffers(1, &m_norm_vbo);
        glDeleteBuffers(1, &m_uv_vbo);
        glDeleteVertexArrays(1, &m_vao);
        delete[] m_blocks;
    }

    void Chunk::init()
    {
        SimplexNoise generator;

        m_blocks = new uint8_t[CX * CY * CZ];

        for (int y = 0; y < CY; y++)
        {
            for (int z = 0; z < CZ; z++)
            {
                for (int x = 0; x < CX; x++)
                {
                    if (y < 255)
                        calc_block(generator, x, y, z);
                    else
                    {
                        set_block(x, y, z, Chunk::block_map["air_block"]);
                    }
                        
                }
            }
        }

        for (int y = 0; y < CY; y++)
        {
            for (int z = 0; z < CZ; z++)
            {
                for (int x = 0; x < CX; x++)
                {
                    int idx = y * CZ * CX + z * CX + x;

                    if (get_block(x, y, z) == Chunk::block_map["air_block"] && get_block(x, y-1, z) == Chunk::block_map["stone"])
                    {
                        // 64 is water level. This is all mostly testing still
                        if (y > 64)
                        {
                            set_block(x, y-1, z, Chunk::block_map["grass_block"]);
                            set_block(x, y-2, z, Chunk::block_map["dirt"]);
                            set_block(x, y-3, z, Chunk::block_map["dirt"]);
                            set_block(x, y-4, z, Chunk::block_map["dirt"]);
                        }
                        else
                        {
                            set_block(x, y-1, z, Chunk::block_map["sand"]);
                            set_block(x, y-2, z, Chunk::block_map["sand"]);
                            set_block(x, y-3, z, Chunk::block_map["sand"]);
                            set_block(x, y-4, z, Chunk::block_map["dirt"]);
                        }
                    }

                    if (y == 64)
                    {
                        set_block(x, y, z, Chunk::block_map["lapis_block"]);
                    }
                }
            }
        }
    }

    // ----- Testing Spline Points -----
    int Chunk::spline_height(float perlin_val)
    {
        Point p1{ -1.0f, 50.0f };
        Point p2{ 0.3f, 80.0f };
        Point p3{ 0.8f, 110.0f };
        Point p4{ 1.0f, 120.0f };

        if (perlin_val < p2.x)
        {
            float slope = (p2.y - p1.y) / (p2.x - p1.x);
            float y_int = p2.y - slope * p2.x;
            return (int)(slope * perlin_val + y_int);
        }
        else if (perlin_val < p3.x)
        {
            float slope = (p3.y - p2.y) / (p3.x - p2.x);
            float y_int = p3.y - slope * p3.x;
            return (int)(slope * perlin_val + y_int);
        }
        else
        {
            float slope = (p4.y - p3.y) / (p4.x - p3.x);
            float y_int = p4.y - slope * p4.x;
            return (int)(slope * perlin_val + y_int);
        }
    }

    float Chunk::spline_squash(float perlin_val)
    {
        Point p1{ -1.0f, 10.0f };
        Point p2{ 0.3f, 40.0f };
        Point p3{ 0.8f, 160.0f };
        Point p4{ 1.0f, 200.0f };

        if (perlin_val < p2.x)
        {
            float slope = (p2.y - p1.y) / (p2.x - p1.x);
            float y_int = p2.y - slope * p2.x;
            return (int)(slope * perlin_val + y_int);
        }
        else if (perlin_val < p3.x)
        {
            float slope = (p3.y - p2.y) / (p3.x - p2.x);
            float y_int = p3.y - slope * p3.x;
            return (int)(slope * perlin_val + y_int);
        }
        else
        {
            float slope = (p4.y - p3.y) / (p4.x - p3.x);
            float y_int = p4.y - slope * p4.x;
            return (slope * perlin_val + y_int);
        }
    }
    // ---------------------------------

    void Chunk::calc_block(const SimplexNoise& generator, int x, int y, int z)
    {
        // int seed = 1203983;
        int seed = 8923039;
        // int seed = 3982347;

        float continentalness = generator.fractal(3, (m_world_position.x + seed + x) / 500.0f, (m_world_position.z + seed + z) / 500.0f);
        int height_offset = spline_height(continentalness);
        float squash_factor = spline_squash(continentalness);

        float val = generator.fractal(3, (m_world_position.x + seed + x) / 100.0f, (seed + y) / 100.0f, (m_world_position.z + seed + z) / 100.0f);
        float bias = 0.0f;

        if (y < height_offset)
        {
            bias = (float)-(height_offset - y) / squash_factor;
        }
        else if (y > height_offset)
        {
            bias = (float)(y-height_offset) / squash_factor;
        }

        if (val <= bias)
        {
            *(m_blocks + y * CZ * CX + z * CX + x) = Chunk::block_map["air_block"];
        }
        else
        {
            *(m_blocks + y * CZ * CX + z * CX + x) = Chunk::block_map["stone"];
        }
    }

    void Chunk::update()
    {
        if (m_changed)
        {
            glm::tvec4<unsigned short>* vertices = new glm::tvec4<unsigned short>[CX * CY * CZ * 6 * 6];
            glm::tvec3<unsigned char>* normals = new glm::tvec3<unsigned char>[CX * CY * CZ * 6 * 6];
            glm::vec2* uvs = new glm::vec2[CX * CY * CZ * 6 * 6];
            
            MC_LOG_INFO("Generating chunk mesh");
            m_changed = false;

            float tex_x, tex_y;

            int vert_idx = 0;
            int uv_idx = 0;
            int norm_idx = 0;
            
            for (int x = 0; x < CX; x++)
            {
                for (int y = 0; y < CY; y++)
                {
                    for (int z = 0; z < CZ; z++)
                    {
                        uint16_t type = *(m_blocks + y * CZ * CX + z * CX + x);
                        tex_x = (1.0f / 16.0f) * (type % 16);
                        tex_y = 1 - (1.0f / 16.0f) * (type / 16);

                        if (type != 0)
                        {
                            if ((x > 0 && *(m_blocks + y * CZ * CX + z * CX + (x - 1)) == 0) || x == 0)
                            {
                                // Face facing negative x
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z+1, type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(-1, 0, 0);


                                std::string texture = block_formats[type].side;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv2;
                            }

                            if ((x < (CX - 1) && *(m_blocks + y * CZ * CX + z * CX + (x + 1)) == 0) || x == (CX - 1))
                            {
                                // Face facing positive x
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z+1, type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(1, 0, 0);

                                std::string texture = block_formats[type].side;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv0;
                            }

                            if ((z > 0 && *(m_blocks + y * CZ * CX + (z - 1) * CX + x) == 0) || z == 0)
                            {
                                // Face facing negative z
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z, type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, -1);

                                std::string texture = block_formats[type].side;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv3;
                            }

                            if ((z < (CZ - 1) && *(m_blocks + y * CZ * CX + (z + 1) * CX + x) == 0) || z == (CZ - 1))
                            {
                                // Face facing positive z
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z+1, type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z+1, type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 0, 1);

                                std::string texture = block_formats[type].side;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv3;
                            }

                            if ((y > 0 && *(m_blocks + (y - 1) * CZ * CX + z * CX + x) == 0) || y == 0)
                            {
                                // Face facing negative y
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z+1, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y, z+1, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y, z+1, -type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, -1, 0);

                                std::string texture = block_formats[type].bottom;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv1;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv3;
                            }


                            if ((y < (CY - 1) && *(m_blocks + (y + 1) * CZ * CX + z * CX + x) == 0) || y == (CY - 1))
                            {
                                // Face facing positive y
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z+1, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x, y+1, z+1, -type);
                                vertices[vert_idx++] = glm::tvec4<unsigned char>(x+1, y+1, z+1, -type);

                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);
                                normals[norm_idx++] = glm::tvec3<unsigned char>(0, 1, 0);

                                std::string texture = block_formats[type].top;
                                TextureFormat texture_uvs = texture_map[texture];
                                uvs[uv_idx++] = texture_uvs.uv3;
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv2;
                                uvs[uv_idx++] = texture_uvs.uv0;
                                uvs[uv_idx++] = texture_uvs.uv1;
                            }
                        }
                    }
                }
            }
            m_vertex_count = vert_idx;

            glGenVertexArrays(1, &m_vao);
            glGenBuffers(1, &m_vert_vbo);
            glGenBuffers(1, &m_norm_vbo);
            glGenBuffers(1, &m_uv_vbo);

            glBindVertexArray(m_vao);
            // vertices
            glBindBuffer(GL_ARRAY_BUFFER, m_vert_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_SHORT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // normals
            glBindBuffer(GL_ARRAY_BUFFER, m_norm_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(normals[0]), normals, GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_BYTE, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // uvs
            glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
            glBufferData(GL_ARRAY_BUFFER, m_vertex_count * sizeof(uvs[0]), uvs, GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(0);

            delete[] vertices;
            delete[] uvs;
            delete[] normals;
        }
    }

    int Chunk::get_block(int x, int y, int z)
    {
        return *(m_blocks + y * CZ * CX + z * CX + x);
    }

    void Chunk::set_block(int x, int y, int z, int block)
    {
        *(m_blocks + y * CZ * CX + z * CX + x) = block;
    }

    glm::ivec3 Chunk::get_chunk_position()
    {
        return m_chunk_position;
    }

    glm::vec3 Chunk::get_world_position()
    {
        return m_world_position;
    }

    unsigned int Chunk::get_vao()
    {
        return m_vao;
    }

    int Chunk::get_vertex_count()
    {
        return m_vertex_count;
    }

    void Chunk::load_block_formats(std::string filepath)
    {
        MC_LOG_WARN("Loading block formats");
        YAMLUTIL::read_block_formats(filepath, Chunk::block_formats, Chunk::block_map);
    }

    void Chunk::load_texture_formats(std::string filepath)
    {
        MC_LOG_WARN("Loading texture formats");
        YAMLUTIL::read_texture_formats(filepath, Chunk::texture_map);
    }
}