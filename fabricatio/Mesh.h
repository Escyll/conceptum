#ifndef MESH_H
#define MESH_H

#pragma once

#include <vector>

class Mesh
{
public:
    Mesh() = default;
    Mesh(const std::vector<float> &vertices, const std::vector<float> &colors, const std::vector<float> &uvs, const std::vector<unsigned int> indices);
    ~Mesh();

    std::vector<float> getVertexBuffer() const;
    std::vector<unsigned int> getIndices() const;

    unsigned int getVao() const;
    void setVao(unsigned int vao);

private:
    std::vector<float> m_vertices, m_colors, m_uvs;
    std::vector<unsigned int> m_indices;
    std::vector<float> m_vertexBuffer;
    unsigned int m_vao = 0;
};

#endif