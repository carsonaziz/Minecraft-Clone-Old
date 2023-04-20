#include "utils/texture_packer.h"

#include "engine/log.h"
#include "utils/yaml_util.h"

#include <string>
#include <filesystem>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "engine/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "engine/stb_image.h"

namespace Minecraft
{
    namespace TEXPACK
    { 
        void pack_textures(const char* dir)
        {
            int file_count = 0;
            for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(dir))
            {
                if (strcmp(dirEntry.path().extension().c_str(), ".png") == 0)
                {
                    file_count++;
                }
            }
            MC_LOG_WARN("Packing {} textures", file_count);

            // TODO:: Remove hard coded values (width and height = 16, num_channels = 4)
            int atlas_size = ceil(sqrt(file_count));
            int atlas_x = 0;
            int atlas_y = 0;
            int tex_width = atlas_size * 16;
            int tex_height = atlas_size * 16;

            std::vector<TextureFormat> texture_formats;

            unsigned char packed_data[tex_width * tex_height * 4];
            for ( int i = 0; i < tex_width * tex_height * 4; i++)
            {
                packed_data[i] = 0;
            }

            int texture_idx = 0;
            for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(dir))
            {
                if (strcmp(dirEntry.path().extension().c_str(), ".png") != 0) continue;
                
                // store texture data in array
                std::string filename = dirEntry.path().filename().c_str();
                int width, height, num_channels;
                unsigned char* data = stbi_load(dirEntry.path().c_str(), &width, &height, &num_channels, 0);
                stbi_image_free(data);
                if (!data) continue;

                atlas_x = texture_idx % atlas_size;
                atlas_y = texture_idx / atlas_size;

                // create TextureFormat for texture
                std::string name = dirEntry.path().stem().string();
                glm::vec2 uv0 = glm::vec2(((float)atlas_x/(float)atlas_size), 1 - ((float)atlas_y/(float)atlas_size) - (1.0f/(float)atlas_size));
                glm::vec2 uv1 = glm::vec2(((float)atlas_x/(float)atlas_size) + (1.0f/(float)atlas_size), 1 - ((float)atlas_y/(float)atlas_size) - (1.0f/(float)atlas_size));
                glm::vec2 uv2 = glm::vec2(((float)atlas_x/(float)atlas_size) + (1.0f/(float)atlas_size), 1 - ((float)atlas_y/(float)atlas_size));
                glm::vec2 uv3 = glm::vec2(((float)atlas_x/(float)atlas_size), 1 - ((float)atlas_y/(float)atlas_size));
                texture_formats.push_back({ name, uv0, uv1, uv2, uv3 });

                // transfer texture data to texture atlas data
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        int atlas_idx = (x * 4) + (atlas_x * 16 * 4) + (y * atlas_size * 16 * 4) + (atlas_y * atlas_size * 16 * 16 * 4);
                        packed_data[atlas_idx + 0] = data[(x * 4) + (y * width * 4) + 0];
                        packed_data[atlas_idx + 1] = data[(x * 4) + (y * width * 4) + 1];
                        packed_data[atlas_idx + 2] = data[(x * 4) + (y * width * 4) + 2];
                        packed_data[atlas_idx + 3] = data[(x * 4) + (y * width * 4) + 3];
                    }
                }

                texture_idx++;
                MC_LOG_WARN("{} of {} textures packed: {}% complete", texture_idx, file_count, (float)texture_idx/(float)file_count * 100);
            }

            YAMLUTIL::write_texture_formats(texture_formats);

            // write texture atlas data to .png
            stbi_write_png("minecraft/res/textures/texture_atlas.png", atlas_size * 16, atlas_size * 16, 4, packed_data, atlas_size * 16 * 4);
        }

        unsigned char* load_texture(std::string filepath, int& width, int& height, int& num_channels)
        {
            stbi_set_flip_vertically_on_load(1);
            unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);
            stbi_image_free(data);

            if (data)
            {
                return data;
            }
            else
            {
                MC_LOG_ERROR("Failed to load texture atlas");
                return NULL;
            }
        } 
    }
}