#ifndef MESH_H
#define MESH_H

#include <vector>
#include <tuple>
#include <cstdint>

#include "Material.h"

class Mesh
{
public:
    using vertex = std::tuple<float, float, float>;
    using uv = std::tuple<float, float>;
    Mesh(const std::vector<vertex> &vertices, const std::vector<uv> &uvs, const std::vector<uint32_t> indices, std::unique_ptr<Material>&& material);

    std::vector<float> getVertexBuffer() const;
    std::vector<uint32_t> getIndices() const;
    Material* getMaterial() const;

    unsigned int getVao() const;
    void setVao(uint32_t vao);

private:
    std::vector<vertex> m_vertices;
    std::vector<uv> m_uvs;
    std::vector<uint32_t> m_indices;
    std::vector<float> m_vertexBuffer;
    unsigned int m_vao = 0;
    std::unique_ptr<Material> m_material;
};

#endif
