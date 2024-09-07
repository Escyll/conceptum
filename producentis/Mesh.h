#ifndef MESH_H
#define MESH_H

#include <vector>
#include <cstdint>
#include <map>
#include <string>

#include "Material.h"
#include "producentis.h"

struct producentis_API SubMesh
{
    uint64_t startIndex;
    uint64_t indexCount;
    std::string materialName;
    uint32_t vao;
};

class producentis_API Mesh
{
public:
    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec2>& uvs,
         const std::vector<glm::vec3>& normals,
         const std::vector<glm::vec3>& tangents,
         const std::vector<glm::vec3>& bitangents,
         const std::vector<uint32_t>& indices,
         const std::vector<SubMesh>& subMeshes,
         std::map<std::string, Material*> materials);
    Mesh(Mesh &&) = default;

    static Mesh fromVertices(const std::vector<glm::vec3> &vertices, Material *material);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    std::vector<glm::vec2> uvs;
    std::vector<SubMesh> subMeshes;
    std::vector<uint32_t> indices;
    std::vector<float> vertexBuffer;
    std::map<std::string, Material*> materialsMap;
};

#endif
