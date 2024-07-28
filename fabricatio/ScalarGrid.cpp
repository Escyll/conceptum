#include "ScalarGrid.h"

ScalarGrid::ScalarGrid(const glm::ivec3 &size)
    : m_size(size), m_sizeXY(size.x * size.y), m_grid(m_sizeXY * size.z)
{
}

float ScalarGrid::scalar(const glm::ivec3 &index) const
{
    return m_grid[this->index(index)];
}

void ScalarGrid::setScalar(const glm::ivec3 &index, float scalar)
{
    m_grid[this->index(index)] = scalar;
}

glm::ivec3 ScalarGrid::size() const
{
    return m_size;
}

uint32_t ScalarGrid::index(const glm::ivec3 &index) const
{
    return index.z * m_sizeXY + index.y * m_size.x + index.x;
}

ScalarGridView::ScalarGridView(const ScalarGrid &grid, const glm::ivec3 &offset)
    : m_grid(grid), m_offset(offset)
{
}

float ScalarGridView::scalar(const glm::ivec3 &index) const
{
    return m_grid.scalar(globalIndex(index));
}

glm::ivec3 ScalarGridView::globalIndex(const glm::ivec3 &index) const
{
    return index + m_offset;
}

glm::vec3 ScalarGridView::interpolate(float value, glm::vec3 &begin, glm::vec3 &end) const
{
    auto result = (glm::vec3)globalIndex(begin) + ((value - scalar(begin)) / (scalar(end) - scalar(begin))) * ((glm::vec3)globalIndex(end) - (glm::vec3)globalIndex(begin));
    return result;
}
