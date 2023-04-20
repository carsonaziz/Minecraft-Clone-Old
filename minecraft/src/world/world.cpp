#include "world/world.h"

#include <vector>

#include "engine/log.h"

namespace Minecraft
{
    Player World::m_player = Player(glm::vec3(0, 70, 0), glm::vec3(0, 0, 0));
    std::unordered_map<glm::vec2, Chunk*> World::m_chunks = std::unordered_map<glm::vec2, Chunk*>();
    std::unordered_map<glm::vec2, Chunk*> World::m_loaded_chunks = std::unordered_map<glm::vec2, Chunk*>();
    std::mutex World::m_mtx = std::mutex();
    std::atomic<bool> World::m_chunk_should_load = true;
    std::unique_lock<std::mutex> World::m_lock = std::unique_lock<std::mutex>(World::m_mtx, std::defer_lock);

    World::World()
    {
        Chunk::load_block_formats("minecraft/res/blockdata/block_format.yaml");
        Chunk::load_texture_formats("minecraft/res/blockdata/texture_format.yaml");

        t_load_chunks = std::thread(worker_load_chunks);
    }

    World::~World()
    {
        m_chunk_should_load = false;
        t_load_chunks.join();
        
        for (auto& it : m_chunks)
        {
            delete it.second;
        }
    }

    void World::update(float dt)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_player.update(dt);
        // procedurally_load_chunk();

        for (auto& it : m_chunks)
        {
            it.second->update();
        }
    }

    void World::procedurally_load_chunk()
    {
        glm::ivec2 player_chunk;

        // std::lock_guard<std::mutex> lock(m_mtx);

        m_lock.lock();
        if (m_player.get_position().x < 0) { player_chunk.x = (int)(m_player.get_position().x / 16.0f) - 1; }
        else { player_chunk.x = (int)(m_player.get_position().x / 16.0f); }
        if (m_player.get_position().z < 0) { player_chunk.y = (int)(m_player.get_position().z / 16.0f) - 1; }
        else { player_chunk.y = (int)(m_player.get_position().z / 16.0f); }
        m_lock.unlock();

        int render_distance = RENDER_DISTANCE / 2;

        m_lock.lock();
        m_loaded_chunks.clear();
        m_lock.unlock();
        for (int z = -render_distance; z < render_distance; z++)
        {
            for (int x = -render_distance; x < render_distance; x++)
            {
                m_lock.lock();
                if (m_chunks.find(glm::vec2(player_chunk.x + x, player_chunk.y + z)) == m_chunks.end())
                {
                    m_lock.unlock();
                    Chunk* chunk = new Chunk(glm::vec3(player_chunk.x + x, 0, player_chunk.y + z));
                    m_lock.lock();
                    m_chunks[glm::vec2(player_chunk.x + x, player_chunk.y + z)] = chunk;
                    m_loaded_chunks[glm::vec2(player_chunk.x + x, player_chunk.y + z)] = chunk;
                    m_lock.unlock();
                }
                else
                {
                    m_loaded_chunks[glm::vec2(player_chunk.x + x, player_chunk.y + z)] = m_chunks.find(glm::vec2(player_chunk.x + x, player_chunk.y + z))->second;
                    m_lock.unlock();
                }
            }
        }

        m_lock.lock();
        std::unordered_map<glm::vec2, Chunk*>::iterator it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (m_loaded_chunks.find(it->first) == m_loaded_chunks.end())
            {
                // delete it->second;
                it = m_chunks.erase(it);
            }
            else
            {
                it++;
            }
        }
        m_lock.unlock();
    }

    void World::worker_load_chunks()
    {
        while(m_chunk_should_load)
        {
            procedurally_load_chunk();
        }
    }

    Player& World::get_player()
    {
        return m_player;
    }

    std::mutex& World::get_mutex()
    {
        return m_mtx;
    }

    std::unique_lock<std::mutex>& World::get_lock()
    {
        return m_lock;
    }

    std::unordered_map<glm::vec2, Chunk*> World::get_chunks()
    {
        return m_chunks;
    }
}