#include "Mesh.h"

#include <cassert>

Mesh::Mesh(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec2> &uvs, const std::vector<glm::vec3> &normals, const std::vector<glm::vec3> &tangents, const std::vector<glm::vec3> &bitangents, const std::vector<uint32_t> &indices, const std::vector<SubMesh> &subMeshes, std::map<std::string, Material *> materials)
    : m_vertices(vertices), m_normals(normals), m_tangents(tangents), m_bitangents(bitangents), m_uvs(uvs), m_indices(indices), m_subMeshes(subMeshes), m_materialsMap(materials)
{
    assert(m_vertices.size() == m_uvs.size() && m_vertices.size() == m_normals.size() && m_vertices.size() == m_tangents.size() && m_vertices.size() == m_bitangents.size());
    auto size = m_vertices.size();
    m_vertexBuffer.reserve((3 + 2 + 3 + 3 + 3) * size);
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

Mesh Mesh::fromVertices(const std::vector<glm::vec3> &vertices, Material *material)
{
    std::vector<uint32_t> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    auto size = (int)vertices.size() - 2;
    for (int i = 0; i < size; i += 3)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        uvs.push_back(glm::vec2{0.0});
        uvs.push_back(glm::vec2{0.0});
        uvs.push_back(glm::vec2{0.0});
        auto v0 = vertices[i];
        auto v1 = vertices[i + 1];
        auto v2 = vertices[i + 2];
        auto e1 = v1 - v0;
        auto e2 = v2 - v0;
        auto normal = glm::normalize(glm::cross(e1, e2));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        glm::vec2 deltaUV1{0.0};
        glm::vec2 deltaUV2{0.0};
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent, bitangent;
        tangent.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
        tangent.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
        tangent.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);
        bitangent.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
        bitangent.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
        bitangent.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
    std::vector<SubMesh> subMeshes{SubMesh{0, indices.size(), material->name, 0}};
    std::map<std::string, Material *> materialMap;
    materialMap[material->name] = material;
    return Mesh(vertices, uvs, normals, tangents, bitangents, indices, subMeshes, {{material->name, material}});
}

std::vector<float> &Mesh::getVertexBuffer()
{
    return m_vertexBuffer;
}

std::vector<uint32_t> &Mesh::getIndices()
{
    return m_indices;
}

std::vector<SubMesh> &Mesh::getSubMeshes()
{
    return m_subMeshes;
}

std::map<std::string, Material *> &Mesh::getMaterials()
{
    return m_materialsMap;
}
