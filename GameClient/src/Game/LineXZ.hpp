#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class LineXZ
{
private:
    glm::vec2 m_pos, m_dir;
    glm::vec2 m_max_xz, m_min_xz;

    glm::vec2 m_intersection_dir;
public:
    LineXZ() = default;
    LineXZ(glm::vec2 pos, glm::vec2 dir, glm::vec2 max_xz, glm::vec2 min_xz);
    ~LineXZ();

    glm::vec2 PerpendicularDirection(glm::vec2 pos);
    LineXZ GetReflectedLineFromX();
};
