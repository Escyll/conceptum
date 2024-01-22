#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <string>

#include "ShaderProgram.h"

class Material
{
public:
    Material(ShaderProgram *program, const std::string &texturePath);
    ~Material();
    const ShaderProgram *getShader() const;
    const unsigned char *getTexture() const;

    unsigned int getTextureId() const;
    void setTextureId(unsigned int id);

    unsigned int getTextureWidth() const;
    unsigned int getTextureHeight() const;
    unsigned int getTextureChannels() const;

private:
    ShaderProgram *m_shader;
    unsigned char *m_textureData;
    unsigned int m_textureId;
    unsigned int m_textureWidth;
    unsigned int m_textureHeight;
    unsigned int m_textureChannels;
};

#endif