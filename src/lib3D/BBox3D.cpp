#include "lib3D\BBox3D.h"

#include <limits>

lib3d::BBox3D::BBox3D()
  : m_min{FLT_MAX},
    m_max{FLT_MIN}
{
}

void lib3d::BBox3D::clear()
{
  m_min.x = m_min.y = m_min.z = FLT_MAX;
  m_max.x = m_max.y = m_max.z = FLT_MIN;
}

void lib3d::BBox3D::add_point(const glm::vec3& p)
{
  if (p.x < m_min.x) m_min.x = p.x;
  if (p.y < m_min.y) m_min.y = p.y;
  if (p.z < m_min.z) m_min.z = p.z;

  if (p.x > m_max.x) m_max.x = p.x;
  if (p.y > m_max.y) m_max.y = p.y;
  if (p.z > m_max.z) m_max.z = p.z;
}

const glm::vec3& lib3d::BBox3D::get_min() const
{
  return m_min;
}

const glm::vec3& lib3d::BBox3D::get_max() const
{
  return m_max;
}
