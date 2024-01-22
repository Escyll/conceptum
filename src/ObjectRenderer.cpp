#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(Material *material)
    : m_material(material)
{
}

ObjectRenderer::~ObjectRenderer()
{
}

Material *ObjectRenderer::getMaterial()
{
    return m_material;
}
