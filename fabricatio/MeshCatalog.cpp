#include <filesystem>
#include <iostream>
#include <sstream>
#include <string_view>
#include <ranges>
#include <tuple>

#include "IO.h"

#include "MeshCatalog.h"

std::unique_ptr<Mesh> loadMesh(const std::filesystem::path& path)
{
    using vertex = std::tuple<float, float, float>;
    using uv = std::tuple<float, float>;
    std::vector<vertex> vertices;
    std::vector <uv> uvs;

    std::vector<vertex> meshVertices;
    std::vector<uv> meshUVs;
    std::vector<uint32_t> meshIndices;
    
    std::istringstream fileContents(IO::readFile(path));
    std::string line;
    while (std::getline(fileContents, line)) {
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word)
        {
            if (word == "mtllib")
            {
                std::string mtllib;
                if (lineStream >> mtllib)
                {
                    // TODO: Read material file
                }
                break;           
            }
            if (word == "v")
            {
                float x, y, z;
                if ((lineStream >> x) && (lineStream >> y) && (lineStream >> z))
                {
                    vertices.push_back({x, y, z});
                }
                break;
            }
            if (word == "vt")
            {
                float u, v;
                if ((lineStream >> u) && (lineStream >> v))
                {
                    uvs.push_back({u, v});
                }
                break;
            }
            if (word == "f")
            {
                std::string v1Packed, v2Packed, v3Packed, v4Packed;
                if ((lineStream >> v1Packed) && (lineStream >> v2Packed) && (lineStream >> v3Packed) && (lineStream >> v4Packed))
                {
                    auto unpack = [&meshVertices, &meshUVs, &vertices, &uvs] (const std::string& vPacked)
                    {
                        std::stringstream ss(vPacked);
                        std::string vp;
                        std::vector<int> vUnpacked;
                        while (std::getline(ss, vp, '/'))
                        {
                            vUnpacked.push_back(std::stoi(vp));
                        }
                        meshVertices.push_back(vertices[vUnpacked[0] - 1]);
                        meshUVs.push_back(uvs[vUnpacked[1] - 1]);
                    };
                    auto startingIndex = meshVertices.size();
                    unpack(v1Packed);                   
                    unpack(v2Packed);
                    unpack(v3Packed);
                    unpack(v4Packed);
                    // Triangle 1
                    meshIndices.push_back(startingIndex);
                    meshIndices.push_back(startingIndex + 1);
                    meshIndices.push_back(startingIndex + 3);
                    // Triangle 2
                    meshIndices.push_back(startingIndex + 1);
                    meshIndices.push_back(startingIndex + 2);
                    meshIndices.push_back(startingIndex + 3);
                }
                break;
            }
            // TODO: g, usemtl and s
        }
    }
    return std::make_unique<Mesh>(meshVertices, meshUVs, meshIndices);
}

void MeshCatalog::loadMeshes(std::string location)
{
    std::string ext = ".obj";
    if (location.empty())
    {
        return;
    }
    if (location.back() != '/')
    {
        location.append("/");
    }
    for (auto &file : std::filesystem::recursive_directory_iterator(location))
    {
        if (file.path().extension() == ".obj")
        {
            std::string key = file.path().string().substr(location.size());
            m_meshes.emplace_back(loadMesh(file.path()));
            m_namedMeshes[key] = m_meshes.back().get();
        }
    }
    for(auto& it : m_namedMeshes)
    {
        std::cout << it.first << std::endl;
    }
}

std::map<std::string, Mesh*>::iterator MeshCatalog::begin()
{
    return m_namedMeshes.begin();
}

std::map<std::string, Mesh*>::iterator MeshCatalog::end()
{
    return m_namedMeshes.end();
}

Mesh* MeshCatalog::getMesh(const std::string& name)
{
    return m_namedMeshes[name];
}
