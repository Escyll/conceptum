#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <memory>
#include <map>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "producentis.h"

class producentis_API Material
{
public:
    using TexturesIterator = std::map<std::string, std::unique_ptr<Texture>>::iterator;
    using FloatsIterator = std::map<std::string, float>::iterator;
    using Vec3sIterator = std::map<std::string, glm::vec3>::iterator;
    Material(const std::string &name);
    Material(Material &&) = default;

    int shader() const;
    std::string name() const;

    void setShader(int shader);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, glm::vec3 value);
    void setTexture(const std::string &name, std::unique_ptr<Texture> texture);

    TexturesIterator texturesBegin();
    TexturesIterator texturesEnd();
    FloatsIterator floatsBegin();
    FloatsIterator floatsEnd();
    Vec3sIterator vec3sBegin();
    Vec3sIterator vec3sEnd();

private:
    std::string m_name;
    int m_shader = -1;
    std::map<std::string, float> m_floats;
    std::map<std::string, glm::vec3> m_vec3s;
    std::map<std::string, std::unique_ptr<Texture>> m_textures;
};

#endif
