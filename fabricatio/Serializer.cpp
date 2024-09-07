#include <cstdlib>
#include <iostream>

#include <fundamentum/Logger.h>

#include <producentis/Camera.h>
#include <producentis/Lights.h>
#include <producentis/Mesh.h>

#include "PlayerControlComponent.h"
#include "Serializer.h"
#include "Transform.h"

#pragma warning(disable : 4996)

namespace {
template<typename Range, typename Value = typename Range::value_type>
std::string join(Range const& elements, const char* const delimiter)
{
    std::ostringstream os;
    auto b = begin(elements), e = end(elements);

    if (b != e)
    {
        std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
        b = prev(e);
    }
    if (b != e)
    {
        os << *b;
    }

    return os.str();
}

class CurrentPathResetter
{
public:
    CurrentPathResetter(const std::filesystem::path& path)
        : m_path(path)
    {}
    ~CurrentPathResetter() { std::filesystem::current_path(m_path); }

private:
    std::filesystem::path m_path;
};

CurrentPathResetter temporarilyChangeCurrentPath(const std::filesystem::path& path)
{
    auto previousPath = std::filesystem::current_path();
    std::filesystem::current_path(path);
    return CurrentPathResetter(previousPath);
}

template<typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;

// Note: Currently ignores endianness, little endian used so far
class BinaryStream
{
public:
    BinaryStream(const std::filesystem::path& file)
        : m_stream(&m_file)
    {
        m_file.open(file, std::ios::out | std::ios::binary);
    }

    ~BinaryStream() { m_file.close(); }

    template<typename T>
    requires arithmetic<T> BinaryStream& operator<<(T value)
    {
        m_stream.write((const char*) &value, sizeof(value));
        return *this;
    }

    BinaryStream& operator<<(entt::entity entity)
    {
        *this << static_cast<char>(0xee) << static_cast<entt::id_type>(entity);
        return *this;
    }

    BinaryStream& operator<<(const glm::vec2& vec2)
    {
        *this << vec2.x << vec2.y;
        return *this;
    }

    BinaryStream& operator<<(const glm::vec3& vec3)
    {
        *this << vec3.x << vec3.y << vec3.z;
        return *this;
    }

    BinaryStream& operator<<(const Transform& transform)
    {
        *this << static_cast<char>(0xc0);
        *this << transform.location << transform.scale << transform.rotation;
        return *this;
    }

    BinaryStream& operator<<(const Camera& camera)
    {
        *this << static_cast<char>(0xc1);
        *this << camera.fov << camera.aspect << camera.nearPlane << camera.farPlane;
        return *this;
    }

    BinaryStream& operator<<(const PlayerControlComponent& playerControl)
    {
        *this << static_cast<char>(0xc2);
        return *this;
    }

    BinaryStream& operator<<(const Light& light)
    {
        *this << light.intensity << light.color;
        return *this;
    }

    BinaryStream& operator<<(const PointLight& pointLight)
    {
        *this << static_cast<char>(0xc3);
        return *this;
    }

    template<typename T>
    BinaryStream& operator<<(const std::vector<T>& vecvec)
    {
        *this << vecvec.size();
        for (const auto& vec : vecvec)
        {
            *this << vec;
        }
        return *this;
    }

    BinaryStream& operator<<(const std::string& text)
    {
        m_stream << text;
        return *this;
    }

    BinaryStream& operator<<(const SubMesh& subMesh)
    {
        *this << subMesh.startIndex << subMesh.indexCount << subMesh.materialName;
        return *this;
    }

    BinaryStream& operator<<(const Mesh& mesh)
    {
        *this << static_cast<char>(0xc4);
        // *this << mesh.vertices << mesh.normals << mesh.tangents << mesh.bitangents << mesh.uvs << mesh.subMeshes << mesh.indices;
        *this << mesh.vertices << mesh.normals << mesh.tangents << mesh.bitangents << mesh.uvs << mesh.subMeshes << mesh.indices;
        return *this;
    }

private:
    std::filebuf m_file;
    std::ostream m_stream;
};

} // namespace

void Serializer::serialize(const entt::registry& registry)
{
#ifdef _WIN32
    std::string folder = std::string(std::getenv("APPDATA")) + "\\fabricatio\\";
#else
    std::string folder = std::getenv("~/.fabricatio/");
#endif

    std::filesystem::create_directory(folder);
    auto currentPathResetter = temporarilyChangeCurrentPath(folder);
    BinaryStream binaryStream("scene.fcsf");

    binaryStream << static_cast<char>(0xfa) << static_cast<char>(0x01);

    auto view = registry.view<entt::entity>();
    std::vector<std::string> sEntities;
    for (auto entity : view)
    {
        binaryStream << entity;
        std::stringstream entitystream;

        std::vector<std::string> sComponents;
        for (auto&& cur : registry.storage())
        {
            entt::id_type type = cur.first;
            if (auto& storage = cur.second; storage.contains(entity))
            {
                std::stringstream componentstream;
                switch (type)
                {
                case entt::type_hash<Transform>():
                    binaryStream << registry.get<Transform>(entity);
                    break;
                case entt::type_hash<Camera>():
                    binaryStream << registry.get<Camera>(entity);
                    break;
                case entt::type_hash<Mesh*>():
                    binaryStream << *registry.get<Mesh*>(entity);
                    break;
                case entt::type_hash<PointLight>():
                    binaryStream << registry.get<PointLight>(entity);
                    break;
                case entt::type_hash<PlayerControlComponent>():
                    binaryStream << PlayerControlComponent{};
                    break;
                default:
                    Log::log() << "Unsupported component for serialization" << Log::end;
                    continue;
                }
                sComponents.push_back(componentstream.str());
            }
        }
        entitystream << join(sComponents, ", ");
        entitystream << "] }";
        sEntities.push_back(entitystream.str());
    }
}
