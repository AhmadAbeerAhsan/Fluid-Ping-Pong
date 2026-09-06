#include "LineXZ.hpp"

LineXZ::LineXZ(glm::vec2 pos, glm::vec2 dir, glm::vec2 max_xz, glm::vec2 min_xz):
    m_pos(pos), m_dir(dir), m_max_xz(max_xz), m_min_xz(min_xz)
{
    if (glm::dot(m_dir, m_dir) > 0.0001)
    {
        m_dir = glm::normalize(m_dir);
    }
    else
    {
        m_dir = glm::vec2(0.0f, 1.0f);
    }
    
}

LineXZ::~LineXZ()
{
}

glm::vec2 LineXZ::PerpendicularDirection(glm::vec2 pos)
{
    glm::vec2 delta = pos - m_pos;

    glm::vec2 projection =
        m_pos + glm::dot(delta, m_dir) * m_dir;

    m_intersection_dir = projection - pos;

    if (projection.y < m_min_xz.y)
    {
        m_intersection_dir = glm::vec2(1000.0f, 1000.0f);
    }
    else if (projection.y > m_max_xz.y)
    {
        m_intersection_dir = glm::vec2(2000.0f, 2000.0f);
    }
    

    return m_intersection_dir;
}

LineXZ LineXZ::GetReflectedLineFromX()
{
    constexpr float epsilon = 0.001f;
    glm::vec2 new_pos;
    glm::vec2 new_dir = glm::vec2(-m_dir.x, m_dir.y);
    if (m_dir.x < -epsilon)
    {
        float t = m_min_xz.x - m_pos.x;
        t /= m_dir.x;

        new_pos = {
            m_min_xz.x,
            m_pos.y + t*m_dir.y
        };
    }
    else if (m_dir.x > epsilon)
    {
        float t = m_max_xz.x - m_pos.x;
        t /= m_dir.x;

        new_pos = {
            m_max_xz.x,
            m_pos.y + t*m_dir.y
        };
    }
    else{
        new_pos = m_pos;
    }
    
    return LineXZ(new_pos, new_dir, m_max_xz, m_min_xz);
}
