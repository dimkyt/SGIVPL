#ifndef _BBox3D_
#define _BBox3D_

#include <glm\vec3.hpp>

namespace lib3d
{
  class BBox3D
  {
  public:
    BBox3D();
    ~BBox3D() = default;

    // Clears the bounding box
    void clear();

    // Expand bounding box, if necessary
    void add_point(const glm::vec3 &p);

    const glm::vec3& get_min() const;
    const glm::vec3& get_max() const;

  private:
    glm::vec3 m_min;
    glm::vec3 m_max;
  };
}

#endif