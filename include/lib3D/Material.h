#ifndef _Material_
#define _Material_

#include <glm\glm.hpp>
#include <string>

namespace lib3d
{

  enum mapType
  {
    MAP_AMBIENT = 0,	     // map_Ka
    MAP_DIFFUSE,		       // map_Kd
    MAP_SPECULAR,          // map_Ks
    MAP_EMISSION,          // map_Ke
    MAP_COUNT
  };

  class Material
  {
  public:
    std::string name;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emission;

    float Ns;

    bool has_texture[MAP_COUNT];
    unsigned int textureID[MAP_COUNT];


    Material();
    ~Material();
  };
}
#endif
