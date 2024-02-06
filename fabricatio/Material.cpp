#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Material::Material(Material&& material)
	: m_shader(material.m_shader)
	, m_textureData(material.m_textureData)
	, m_textureId(material.m_textureId)
	, m_textureWidth(material.m_textureWidth)
	, m_textureHeight(material.m_textureHeight)
	, m_textureChannels(material.m_textureChannels)
{
	material.m_textureData = nullptr;
}

Material::Material(int shaderProgram, const std::string& texturePath)
	: m_shader(shaderProgram)
	, m_textureId(0)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	m_textureData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	m_textureWidth = width;
	m_textureHeight = height;
	m_textureChannels = nrChannels;
}

Material::~Material()
{
	if (m_textureData != nullptr)
	{
		stbi_image_free(m_textureData);
	}
}

Material& Material::operator=(Material&& material)
{
	m_shader = material.m_shader;
	m_textureData = material.m_textureData;
	material.m_textureData = nullptr;
	m_textureId = material.m_textureId;
	m_textureWidth = material.m_textureWidth;
	m_textureHeight = material.m_textureHeight;
	m_textureChannels = material.m_textureChannels;
	return *this;
}

int Material::getShader() const
{
	return m_shader;
}

const unsigned char* Material::getTexture() const
{
	return m_textureData;
}

unsigned int Material::getTextureId() const
{
	return m_textureId;
}

void Material::setTextureId(unsigned int id)
{
	m_textureId = id;
}

unsigned int Material::getTextureWidth() const
{
	return m_textureWidth;
}

unsigned int Material::getTextureHeight() const
{
	return m_textureHeight;
}

unsigned int Material::getTextureChannels() const
{
	return m_textureChannels;
}
