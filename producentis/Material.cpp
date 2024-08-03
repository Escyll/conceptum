#include "Material.h"

Material::Material(const std::string &name)
    : m_name(name)
{
}

int Material::shader() const
{
    return m_shader;
}

std::string Material::name() const
{
    return m_name;
}

void Material::setShader(int shader)
{
    m_shader = shader;
}

void Material::setFloat(const std::string &name, float value)
{
    m_floats[name] = value;
}

void Material::setVec3(const std::string &name, glm::vec3 value)
{
    m_vec3s[name] = value;
}

void Material::setTexture(const std::string &name, std::unique_ptr<Texture> texture)
{
    m_textures[name] = std::move(texture);
}

Material::TexturesIterator Material::texturesBegin()
{
    return m_textures.begin();
}

Material::TexturesIterator Material::texturesEnd()
{
    return m_textures.end();
}

Material::FloatsIterator Material::floatsBegin()
{
    return m_floats.begin();
}

Material::FloatsIterator Material::floatsEnd()
{
    return m_floats.end();
}

Material::Vec3sIterator Material::vec3sBegin()
{
    return m_vec3s.begin();
}

Material::Vec3sIterator Material::vec3sEnd()
{
    return m_vec3s.end();
}