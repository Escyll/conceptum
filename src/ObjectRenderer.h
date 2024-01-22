#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#pragma once

#include "Material.h"

class ObjectRenderer
{
public:
    ObjectRenderer(Material *material);
    ~ObjectRenderer();

    Material *getMaterial();

private:
    Material *m_material;
};

#endif