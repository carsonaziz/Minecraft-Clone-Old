#include "graphics/texture_atlas.h"

#include <glad/glad.h>

#include "engine/log.h"
#include "utils/texture_packer.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "engine/stb_image.h"

namespace Minecraft
{
    TextureAtlas::TextureAtlas(const std::string& file_path) : m_file_path(file_path)
    {
        generate_texture();
    }

    unsigned int TextureAtlas::get()
    {
        return m_texture;
    }

    void TextureAtlas::generate_texture()
    {
        unsigned char* data = TEXPACK::load_texture(m_file_path, m_width, m_height, m_num_channels);
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}