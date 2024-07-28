#ifndef TEXTURE
#define TEXTURE

#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>

#include "producentis.h"

class producentis_API Texture
{
public:
    Texture(unsigned char *data, uint32_t width, uint32_t height, uint8_t channels);
    Texture(Texture &&texture) = delete;
    Texture *operator=(Texture &&texture) = delete;
    ~Texture();
    unsigned char *getData() const;
    void setId(uint32_t id);
    uint32_t getId() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint8_t getChannels() const;

private:
    unsigned char *m_data;
    uint32_t m_width, m_height, m_id;
    uint8_t m_channels;
};

#endif
