#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <string>

class Material
{
public:
    Material() = default;
    Material(Material&) = delete;
    Material(Material&&);
    Material(int program, const std::string &texturePath);
    ~Material();
    Material& operator=(Material&& material);
    int getShader() const;
    const unsigned char *getTexture() const;

    unsigned int getTextureId() const;
    void setTextureId(unsigned int id);

    unsigned int getTextureWidth() const;
    unsigned int getTextureHeight() const;
    unsigned int getTextureChannels() const;

private:
    int m_shader;
    unsigned char *m_textureData;
    unsigned int m_textureId;
    unsigned int m_textureWidth;
    unsigned int m_textureHeight;
    unsigned int m_textureChannels;
};

#endif
