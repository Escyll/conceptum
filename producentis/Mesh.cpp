#include "Mesh.h"

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<uv> &uvs, const std::vector<uint32_t> indices, std::unique_ptr<Material>&& material)
    : m_vertices(vertices), m_uvs(uvs), m_indices(indices), m_material(std::move(material))
{
    m_vertexBuffer.reserve(5*m_vertices.size() + 2*m_uvs.size());
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_vertexBuffer.push_back(std::get<0>(m_vertices[i]));
        m_vertexBuffer.push_back(std::get<1>(m_vertices[i]));
        m_vertexBuffer.push_back(std::get<2>(m_vertices[i]));
        m_vertexBuffer.push_back(std::get<0>(m_uvs[i]));
        m_vertexBuffer.push_back(std::get<1>(m_uvs[i]));
    }
}

std::vector<float> Mesh::getVertexBuffer() const
{
    return m_vertexBuffer;
}

std::vector<uint32_t> Mesh::getIndices() const
{
    return m_indices;
}

Material* Mesh::getMaterial() const
{
    return m_material.get();
}

uint32_t Mesh::getVao() const
{
    return m_vao;
}

void Mesh::setVao(uint32_t vao)
{
    m_vao = vao;
}
