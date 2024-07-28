#include "producentis/Mesh.h"
#include "ScalarGrid.h"

class MarchingCubes
{
public:
    static Mesh *March(const ScalarGrid &scalarGrid, float isoLevel);
};