#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Material::Material(Material&& material)
	: m_shader(material.m_shader)
    , m_name(material.m_name)
	, m_diffuseTexture(std::move(material.m_diffuseTexture))
{
}

Material::Material(const std::string& name, std::unique_ptr<Texture>&& diffuseTexture)
	: m_shader(0)
    , m_name(name)
	, m_diffuseTexture(std::move(diffuseTexture))
{
}

Material& Material::operator=(Material&& material)
{
	m_shader = material.m_shader;
    m_name = material.m_name;
	m_diffuseTexture = std::move(material.m_diffuseTexture);
	return *this;
}

int Material::getShader() const
{
	return m_shader;
}

void Material::setShader(int shader)
{
    m_shader = shader;
}

std::string Material::getName() const
{
    return m_name;
}

Texture* Material::getDiffuseTexture() const
{
	return m_diffuseTexture.get();
}
