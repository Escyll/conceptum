#ifndef MESH_CATALOG
#define MESH_CATALOG

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "producentis/Mesh.h"

class MeshCatalog
{
public:
    void loadMeshes(std::string location);
    Mesh* getMesh(const std::string& name);
    std::map<std::string, Mesh*>::iterator begin();
    std::map<std::string, Mesh*>::iterator end();

private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::map<std::string, Mesh*> m_namedMeshes;
};

#endif // MESH_CATALOG
