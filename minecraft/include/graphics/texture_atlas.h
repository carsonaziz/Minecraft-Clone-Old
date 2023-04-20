#include <string>

namespace Minecraft
{
    class TextureAtlas
    {
    private:
        std::string m_file_path;

        int m_width, m_height, m_num_channels;
        unsigned int m_texture;

    private:
        void generate_texture();
    public:
        TextureAtlas(const std::string& file_path);

        unsigned int get();
    };
}