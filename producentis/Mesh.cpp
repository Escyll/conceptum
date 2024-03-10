#include "Mesh.h"

#include <cassert>

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents, const std::vector<glm::vec3>& bitangents, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes, std::map<std::string, std::unique_ptr<Material>>&& materials)
    : m_vertices(vertices), m_normals(normals), m_tangents(tangents), m_bitangents(bitangents), m_uvs(uvs), m_indices(indices), m_subMeshes(subMeshes), m_materialsMap(std::move(materials))
{
    assert(m_vertices.size() == m_uvs.size() && m_vertices.size() == m_normals.size() && m_vertices.size() == m_tangents.size() && m_vertices.size() == m_bitangents.size());
    auto size = m_vertices.size();
    m_vertexBuffer.reserve((3+2+3+3+3)*size);
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_vertexBuffer.push_back(m_vertices[i].x);
        m_vertexBuffer.push_back(m_vertices[i].y);
        m_vertexBuffer.push_back(m_vertices[i].z);
        m_vertexBuffer.push_back(m_uvs[i].x);
        m_vertexBuffer.push_back(m_uvs[i].y);
        m_vertexBuffer.push_back(m_normals[i].x);
        m_vertexBuffer.push_back(m_normals[i].y);
        m_vertexBuffer.push_back(m_normals[i].z);
        m_vertexBuffer.push_back(m_tangents[i].x);
        m_vertexBuffer.push_back(m_tangents[i].y);
        m_vertexBuffer.push_back(m_tangents[i].z);
        m_vertexBuffer.push_back(m_bitangents[i].x);
        m_vertexBuffer.push_back(m_bitangents[i].y);
        m_vertexBuffer.push_back(m_bitangents[i].z);
    }
}

std::vector<float>& Mesh::getVertexBuffer()
{
    return m_vertexBuffer;
}

std::vector<uint32_t>& Mesh::getIndices()
{
    return m_indices;
}

std::vector<SubMesh>& Mesh::getSubMeshes()
{
    return m_subMeshes;
}

std::map<std::string, std::unique_ptr<Material>>& Mesh::getMaterials()
{
    return m_materialsMap;
}
