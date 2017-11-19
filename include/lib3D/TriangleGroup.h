#ifndef _TriangleGroup_
#define _TriangleGroup_

#include "lib3D\BBox3D.h"

#include "glm\vec3.hpp"

#include <vector>


namespace lib3d
{
  class TriangleGroup
  {
  public:
    TriangleGroup();
    ~TriangleGroup();

    void add_vertex_index(unsigned int);
    void add_texture_index(unsigned int);
    void add_normal_index(unsigned int);

    void add_face();

    void expand_bounding_box(const glm::vec3& p);

    void set_material_index(unsigned int);
    void set_texture_flag(bool);
    void set_normal_flag(bool);

    unsigned int get_vertex_index(unsigned int) const;
    unsigned int get_texture_index(unsigned int) const;
    unsigned int get_normal_index(unsigned int) const;

    unsigned int get_num_vertex_index() const;
    unsigned int get_num_texture_index() const;
    unsigned int get_num_normal_index() const;

    unsigned int get_num_faces() const;

    unsigned int get_material_index() const;

    bool has_texture() const;

    void trim_vectors();




  private:
    std::vector<unsigned int> m_vertex_index;
    std::vector<unsigned int> m_texture_index;
    std::vector<unsigned int> m_normal_index;

    BBox3D m_bounding_box;

    unsigned int m_material_index;

    bool m_has_texture;
    bool m_has_normal;

    unsigned int m_num_faces;
  };
}

#endif
