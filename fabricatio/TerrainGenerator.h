#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <producentis/Mesh.h>

class TerrainGenerator
{
public:
    TerrainGenerator(int seed);
    Mesh* generateTerrainMesh() const;

private:
    int m_seed;
};

#endif // TERRAINGENERATOR_H
