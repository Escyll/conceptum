#include <cstdlib>

#include "Texture.h"

Texture::Texture(unsigned char* data, uint32_t width, uint32_t height, uint8_t channels)
    : m_data(data)
    , m_width(width)
    , m_height(height)
    , m_id(0)
    , m_channels(channels)
{
}

Texture::~Texture()
{
    free(m_data);
}

unsigned char* Texture::getData() const
{
    return m_data;
}

void Texture::setId(uint32_t id)
{
    m_id = id;
}

uint32_t Texture::getId() const
{
    return m_id;
}

uint32_t Texture::getWidth() const
{
    return m_width;
}

uint32_t Texture::getHeight() const
{
    return m_height;
}

uint8_t Texture::getChannels() const
{
    return m_channels;
}
