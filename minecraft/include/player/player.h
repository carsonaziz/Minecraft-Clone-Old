#pragma once

#include "glm/glm.hpp"

namespace Minecraft
{
    class Player
    {
    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_direction;

        const float m_walk_speed = 8.0f;
    
    public:
        Player(const glm::vec3& position, const glm::vec3& rotation);

        void update(float dt);

        glm::vec3 get_position();
        glm::vec3 get_rotation();
        glm::vec3 get_direction();
    };
}