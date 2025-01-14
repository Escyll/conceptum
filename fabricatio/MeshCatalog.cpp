#include <filesystem>
#include <iostream>
#include <sstream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "fundamentum/Logger.h"

#include "IO.h"
#include "MeshCatalog.h"

bool replace(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

glm::vec3 readVec3(std::istringstream &stream)
{
    glm::vec3 result;
    stream >> result.x;
    stream >> result.y;
    stream >> result.z;
    return result;
}

std::map<std::string, Material *> loadMaterial(const std::filesystem::path &path)
{
    std::map<std::string, Material *> result;
    Material *currentMaterial = nullptr;

    std::istringstream fileContents(IO::readFile(path.string()));
    std::string line;
    while (std::getline(fileContents, line))
    {
        ltrim(line);
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word)
        {
            ltrim(word);
            if (word == "newmtl")
            {
                std::string name;
                std::getline(lineStream, name);
                Log::log() << "\n" << name << Log::end;
                result[name] = new Material(name);
                currentMaterial = result[name];
            }
            else if (word == "Ns")
            {
                float specularExponent;
                lineStream >> specularExponent;
                currentMaterial->setFloat("specularExponent", specularExponent);
                Log::log() << "SpecularExponent: " << specularExponent << Log::end;
            }
            else if (word == "Ka")
            {
                currentMaterial->setVec3("ambientColor", readVec3(lineStream));
            }
            else if (word == "Kd")
            {
                currentMaterial->setVec3("diffuseColor", readVec3(lineStream));
            }
            else if (word == "Ks")
            {
                currentMaterial->setVec3("specularColor", readVec3(lineStream));
            }
            else if (word == "map_Kd")
            {
                std::string mapName;
                while (lineStream >> mapName)
                {
                    if (mapName == "-s")
                    {
                        currentMaterial->setVec3("diffuseScale", readVec3(lineStream));
                    }
                    else
                    {
                        std::string rest;
                        std::getline(lineStream, rest);
                        mapName += rest;
                        Log::log() << mapName << Log::end;
                        auto texturePath = (path.parent_path() / mapName).string();
                        replace(texturePath, R"(\)", R"(/)");
                        int width, height, nrChannels;
                        stbi_set_flip_vertically_on_load(true);
                        unsigned char *textureData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
                        currentMaterial->setTexture("diffuseTexture", std::make_unique<Texture>(textureData, width, height, nrChannels));
                    }
                }
            }
            else if (word == "map_Bump")
            {
                std::string mapName;
                while (lineStream >> mapName)
                {
                    if (mapName == "-bm")
                    {
                        float normalStrength;
                        lineStream >> normalStrength;
                        currentMaterial->setFloat("normalStrength", normalStrength);
                        Log::log() << "-bm" << " " << normalStrength << Log::end;
                    }
                    else if (mapName == "-s")
                    {
                        currentMaterial->setVec3("normalScale", readVec3(lineStream));
                    }
                    else
                    {
                        std::string rest;
                        std::getline(lineStream, rest);
                        mapName += rest;
                        Log::log() << mapName << Log::end;
                        auto texturePath = (path.parent_path() / mapName).string();
                        replace(texturePath, R"(\)", R"(/)");
                        int width, height, nrChannels;
                        stbi_set_flip_vertically_on_load(true);
                        unsigned char *textureData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
                        if (stbi_failure_reason())
                            Log::log() << stbi_failure_reason() << Log::end;
                        currentMaterial->setTexture("normalMap", std::make_unique<Texture>(textureData, width, height, nrChannels));
                    }
                }
            }
            std::getline(lineStream, word);
        }
    }
    return std::move(result);
}

std::unique_ptr<Mesh> loadMesh(const std::filesystem::path &path)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::vector<glm::vec3> meshVertices;
    std::vector<glm::vec3> meshNormals;
    std::vector<glm::vec3> meshTangents;
    std::vector<glm::vec3> meshBitangents;
    std::vector<glm::vec2> meshUVs;
    std::vector<uint32_t> meshIndices;
    std::map<std::string, Material *> meshMaterialsMap;
    std::vector<SubMesh> meshSubMeshes;

    std::istringstream fileContents(IO::readFile(path.string()));
    std::string line;
    while (std::getline(fileContents, line))
    {
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word)
        {
            if (word == "mtllib")
            {
                std::string mtllib;
                if (lineStream >> mtllib)
                {
                    auto relativeMtllibPath = (path.parent_path() / mtllib).string();
                    replace(relativeMtllibPath, R"(\)", R"(/)");
                    Log::log() << relativeMtllibPath << Log::end;
                    meshMaterialsMap = loadMaterial(relativeMtllibPath);
                }
                break;
            }
            if (word == "usemtl")
            {
                std::string matName;
                std::getline(lineStream, matName);
                if (meshSubMeshes.size() > 0)
                {
                    auto &lastSubMesh = meshSubMeshes.back();
                    lastSubMesh.indexCount = meshIndices.size() - lastSubMesh.startIndex;
                }
                meshSubMeshes.push_back({meshIndices.size(), 0, matName, 0});
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
            if (word == "vn")
            {
                float x, y, z;
                if ((lineStream >> x) && (lineStream >> y) && (lineStream >> z))
                {
                    normals.push_back({x, y, z});
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
                std::string v1Packed, v2Packed, v3Packed;
                if ((lineStream >> v1Packed) && (lineStream >> v2Packed) && (lineStream >> v3Packed))
                {
                    struct UnpackData
                    {
                        glm::vec3 vertex;
                        glm::vec3 normal;
                        glm::vec2 uv;
                    };
                    auto unpack = [&meshVertices, &meshUVs, &meshNormals, &vertices, &uvs, &normals](const std::string &vPacked) -> UnpackData
                    {
                        std::stringstream ss(vPacked);
                        std::string vp;
                        std::vector<int> vUnpacked;
                        while (std::getline(ss, vp, '/'))
                        {
                            vUnpacked.push_back(std::stoi(vp));
                        }
                        return {vertices[vUnpacked[0] - 1], normals[vUnpacked[2] - 1], uvs[vUnpacked[1] - 1]};
                    };
                    auto v1Unpacked = unpack(v1Packed);
                    auto v2Unpacked = unpack(v2Packed);
                    auto v3Unpacked = unpack(v3Packed);
                    meshVertices.push_back(v1Unpacked.vertex);
                    meshVertices.push_back(v2Unpacked.vertex);
                    meshVertices.push_back(v3Unpacked.vertex);
                    meshNormals.push_back(v1Unpacked.normal);
                    meshNormals.push_back(v2Unpacked.normal);
                    meshNormals.push_back(v3Unpacked.normal);
                    meshUVs.push_back(v1Unpacked.uv);
                    meshUVs.push_back(v2Unpacked.uv);
                    meshUVs.push_back(v3Unpacked.uv);

                    auto e1 = v2Unpacked.vertex - v1Unpacked.vertex;
                    auto e2 = v3Unpacked.vertex - v1Unpacked.vertex;
                    auto deltaUV1 = v2Unpacked.uv - v1Unpacked.uv;
                    auto deltaUV2 = v3Unpacked.uv - v1Unpacked.uv;
                    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
                    glm::vec3 tangent, bitangent;
                    tangent.x = f * (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
                    tangent.y = f * (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
                    tangent.z = f * (deltaUV2.y * e1.z - deltaUV1.y * e2.z);
                    bitangent.x = f * (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
                    bitangent.y = f * (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
                    bitangent.z = f * (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);
                    meshTangents.push_back(tangent);
                    meshTangents.push_back(tangent);
                    meshTangents.push_back(tangent);
                    meshBitangents.push_back(bitangent);
                    meshBitangents.push_back(bitangent);
                    meshBitangents.push_back(bitangent);
                    meshIndices.push_back(meshVertices.size() - 3);
                    meshIndices.push_back(meshVertices.size() - 2);
                    meshIndices.push_back(meshVertices.size() - 1);
                }
                break;
            }
            // TODO: g and s
        }
    }
    if (meshSubMeshes.size() > 0)
    {
        auto &lastSubMesh = meshSubMeshes.back();
        lastSubMesh.indexCount = meshIndices.size() - lastSubMesh.startIndex;
    }

    return std::make_unique<Mesh>(meshVertices,
                                  meshUVs,
                                  meshNormals,
                                  meshTangents,
                                  meshBitangents,
                                  meshIndices,
                                  meshSubMeshes,
                                  std::move(meshMaterialsMap));
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
            replace(key, R"(\)", R"(/)");
            m_meshes.emplace_back(loadMesh(file.path()));
            m_namedMeshes[key] = m_meshes.back().get();
        }
    }
    for (auto &it : m_namedMeshes)
    {
        Log::log() << it.first << Log::end;
    }
}

std::map<std::string, Mesh *>::iterator MeshCatalog::begin()
{
    return m_namedMeshes.begin();
}

std::map<std::string, Mesh *>::iterator MeshCatalog::end()
{
    return m_namedMeshes.end();
}

Mesh *MeshCatalog::getMesh(const std::string &name)
{
    return m_namedMeshes[name];
}
