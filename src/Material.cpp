#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Material::Material(ShaderProgram *shaderProgram, const std::string &texturePath)
    : m_shader(shaderProgram)
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
    stbi_image_free(m_textureData);
}

const ShaderProgram *Material::getShader() const
{
    return m_shader;
}

const unsigned char *Material::getTexture() const
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
