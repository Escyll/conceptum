#include "producentis/Mesh.h"
#include "ScalarGrid.h"

class MarchingCubes
{
public:
    static Mesh* march(const ScalarGrid& scalarGrid, float isoLevel);
};
