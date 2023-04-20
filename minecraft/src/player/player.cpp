#include "player/player.h"

#include "GLFW/glfw3.h"
#include <cstdio>

#include "engine/log.h"
#include "input/keyboard.h"
#include "input/mouse.h"

namespace Minecraft
{
    Player::Player(const glm::vec3& position, const glm::vec3& rotation) : m_position(position), m_rotation(rotation)
    {
        m_direction.x = glm::cos(rotation.x) * glm::sin(rotation.z);
        m_direction.y = glm::sin(rotation.x);
        m_direction.z = glm::cos(rotation.x) * glm::cos(rotation.z);
    }

    void Player::update(float dt)
    {
        Mouse::update();
        float x = glm::radians((float)Mouse::get_dy()) * 0.1f;
        if (m_rotation.x - x > 3.14f/2.0f)
        {
            m_rotation.x -= 0;
        }
        else if (m_rotation.x - x < -3.14f/2.0f)
        {
            m_rotation.x -= 0;
        }
        else
        {
            m_rotation.x -= x;
        }
        m_rotation.z -= glm::radians((float)Mouse::get_dx()) * 0.1f;

        m_direction.x = glm::cos(m_rotation.x) * glm::sin(m_rotation.z);
        m_direction.y = glm::sin(m_rotation.x);
        m_direction.z = glm::cos(m_rotation.x) * glm::cos(m_rotation.z);

        float normalized_xz = glm::sqrt((m_direction.x * m_direction.x) + (m_direction.z * m_direction.z));

        if (Keyboard::is_down(GLFW_KEY_W))
        {
            // m_direction x and z are normalized so walk speed doesn't slow down when looking down or up
            m_position.x += m_direction.x / normalized_xz * dt * m_walk_speed;
            m_position.z += m_direction.z / normalized_xz * dt * m_walk_speed;
        }
        if (Keyboard::is_down(GLFW_KEY_S))
        {
            m_position.x -= m_direction.x / normalized_xz * dt * m_walk_speed;
            m_position.z -= m_direction.z / normalized_xz * dt *  m_walk_speed;
        }
        if (Keyboard::is_down(GLFW_KEY_A))
        {
            m_position.x += m_direction.z / normalized_xz * dt *  m_walk_speed;
            m_position.z -= m_direction.x / normalized_xz * dt *  m_walk_speed;
        }
        if (Keyboard::is_down(GLFW_KEY_D))
        {
            m_position.x -= m_direction.z / normalized_xz * dt *  m_walk_speed;
            m_position.z += m_direction.x / normalized_xz * dt *  m_walk_speed;
        }
        if (Keyboard::is_down(GLFW_KEY_LEFT_SHIFT))
        {
            m_position.y -= dt * m_walk_speed;
        }
        if (Keyboard::is_down(GLFW_KEY_SPACE))
        {
            m_position.y += dt * m_walk_speed;
        }
    }

    glm::vec3 Player::get_position()
    {
        return m_position;
    }

    glm::vec3 Player::get_rotation()
    {
        return m_rotation;
    }

    glm::vec3 Player::get_direction()
    {
        return m_direction;
    }
}