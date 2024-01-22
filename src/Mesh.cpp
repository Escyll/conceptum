#include "Mesh.h"

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<float> &colors, const std::vector<float> &uvs, const std::vector<unsigned int> indices)
    : m_vertices(vertices), m_colors(colors), m_uvs(uvs), m_indices(indices)
{
    m_vertexBuffer.reserve(m_vertices.size() + m_colors.size() + m_uvs.size());
    for (int i = 0; i < (m_vertices.size() / 3); i++)
    {
        m_vertexBuffer.push_back(m_vertices[i * 3]);
        m_vertexBuffer.push_back(m_vertices[i * 3 + 1]);
        m_vertexBuffer.push_back(m_vertices[i * 3 + 2]);
        m_vertexBuffer.push_back(m_colors[i * 3]);
        m_vertexBuffer.push_back(m_colors[i * 3 + 1]);
        m_vertexBuffer.push_back(m_colors[i * 3 + 2]);
        m_vertexBuffer.push_back(m_uvs[i * 2]);
        m_vertexBuffer.push_back(m_uvs[i * 2 + 1]);
    }
}

Mesh::~Mesh()
{
}

std::vector<float> Mesh::getVertexBuffer() const
{
    return m_vertexBuffer;
}

std::vector<unsigned int> Mesh::getIndices() const
{
    return m_indices;
}

unsigned int Mesh::getVao() const
{
    return m_vao;
}

void Mesh::setVao(unsigned int vao)
{
    m_vao = vao;
}
