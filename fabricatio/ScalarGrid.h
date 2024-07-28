#ifndef SCALAR_GRID_H
#define SCALAR_GRID_H

#include <vector>
#include <glm/glm.hpp>

class ScalarGrid
{
public:
    ScalarGrid(const glm::ivec3 &size);
    float scalar(const glm::ivec3 &size) const;
    void setScalar(const glm::ivec3 &index, float scalar);
    glm::ivec3 size() const;

private:
    uint32_t index(const glm::ivec3 &index) const;

private:
    glm::ivec3 m_size;
    uint32_t m_sizeXY;
    std::vector<float> m_grid;
};

class ScalarGridView
{
public:
    ScalarGridView(const ScalarGrid &grid, const glm::ivec3 &offset);
    float scalar(const glm::ivec3 &index) const;
    glm::ivec3 globalIndex(const glm::ivec3 &index) const;
    glm::vec3 interpolate(float value, glm::vec3 &begin, glm::vec3 &end) const;

private:
    const ScalarGrid &m_grid;
    glm::ivec3 m_offset;
};

#endif