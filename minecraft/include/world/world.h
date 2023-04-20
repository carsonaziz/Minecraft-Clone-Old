#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <atomic>

#include "glm/glm.hpp"

#include "world/chunk.h"
#include "player/player.h"

#define RENDER_DISTANCE 16

namespace std {

template<>
struct hash<glm::vec2> 
{
    size_t operator()(const glm::vec2& vec) const 
    {
        unsigned int b    = 378551;
        unsigned int a    = 63689;
        unsigned int hash_result = 0;
        unsigned int i    = 0;

        for (i = 0; i < 20; ++i)
        {
            hash_result = hash_result * a + vec.x;
            hash_result = hash_result * a + vec.y;
            a           = a * b;
        }

        return (size_t)hash_result;
    }
};

}

namespace Minecraft
{
    class World
    {
    private:
        static std::mutex m_mtx;
        std::thread t_load_chunks;
        static std::unique_lock<std::mutex> m_lock;

        static Player m_player;// = Player(glm::vec3(0, 60, 0), glm::vec3(0, 0, 0));

        static std::unordered_map<glm::vec2, Chunk*> m_chunks;// = std::unordered_map<glm::vec2, Chunk*>();
        static std::unordered_map<glm::vec2, Chunk*> m_loaded_chunks;// = std::unordered_map<glm::vec2, Chunk*>();

        static std::atomic<bool> m_chunk_should_load;

    private:
        static void procedurally_load_chunk();
        static void worker_load_chunks();
    public:
        World();
        ~World();

        static void update(float dt);

        static std::unordered_map<glm::vec2, Chunk*> get_chunks();
        static Player& get_player();
        static std::mutex& get_mutex();
        static std::unique_lock<std::mutex>& get_lock();
    };
}