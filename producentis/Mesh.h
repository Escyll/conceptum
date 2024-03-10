#ifndef MESH_H
#define MESH_H

#include <vector>
#include <tuple>
#include <cstdint>
#include <memory>
#include <map>
#include <string>

#include "Material.h"

struct SubMesh {
    uint64_t startIndex;
    uint64_t indexCount;
    std::string materialName;
    uint32_t vao;
};

class Mesh
{
public:
    Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents, const std::vector<glm::vec3>& bitangents, const std::vector<uint32_t>& indices, const std::vector<SubMesh>& subMeshes, std::map<std::string, std::unique_ptr<Material>>&& materials);

    std::vector<float>& getVertexBuffer();
    std::vector<uint32_t>& getIndices();
    std::vector<SubMesh>& getSubMeshes();
    std::map<std::string, std::unique_ptr<Material>>& getMaterials();

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_tangents;
    std::vector<glm::vec3> m_bitangents;
    std::vector<glm::vec2> m_uvs;
    std::vector<SubMesh> m_subMeshes;
    std::vector<uint32_t> m_indices;
    std::vector<float> m_vertexBuffer;
    std::map<std::string, std::unique_ptr<Material>> m_materialsMap;
};

#endif
