#include "lib3d\TriangleGroup.h"

lib3d::TriangleGroup::TriangleGroup()
  : m_has_texture(false),
    m_has_normal(false),
    m_num_faces(0)
{
}

lib3d::TriangleGroup::~TriangleGroup()
{
}

void lib3d::TriangleGroup::add_vertex_index(unsigned int index) {
  m_vertex_index.push_back(index);
}

void lib3d::TriangleGroup::add_texture_index(unsigned int index) {
  m_texture_index.push_back(index);
}

void lib3d::TriangleGroup::add_normal_index(unsigned int index) {
  m_normal_index.push_back(index);
}

void lib3d::TriangleGroup::add_face() {
  m_num_faces++;
}

void lib3d::TriangleGroup::expand_bounding_box(const glm::vec3 & p)
{
  m_bounding_box.add_point(p);
}

void lib3d::TriangleGroup::set_material_index(unsigned int index) {
  m_material_index = index;
}

void lib3d::TriangleGroup::set_texture_flag(bool has_texture) {
  m_has_texture = has_texture;
}

void lib3d::TriangleGroup::set_normal_flag(bool has_normal) {
  m_has_normal = has_normal;
}

unsigned int lib3d::TriangleGroup::get_vertex_index(unsigned int index) const {
  return m_vertex_index[index];
}

unsigned int lib3d::TriangleGroup::get_texture_index(unsigned int index) const {
  return m_texture_index[index];
}

unsigned int lib3d::TriangleGroup::get_normal_index(unsigned int index) const {
  return m_normal_index[index];
}

unsigned int lib3d::TriangleGroup::get_num_vertex_index() const {
  return static_cast<unsigned int>(m_vertex_index.size());
}

unsigned int lib3d::TriangleGroup::get_num_texture_index() const {
  return static_cast<unsigned int>(m_texture_index.size());
}

unsigned int lib3d::TriangleGroup::get_num_normal_index() const {
  return static_cast<unsigned int>(m_normal_index.size());
}

unsigned int lib3d::TriangleGroup::get_num_faces() const {
  return m_num_faces;
}

unsigned int lib3d::TriangleGroup::get_material_index() const {
  return m_material_index;
}

bool lib3d::TriangleGroup::has_texture() const
{
  return m_has_texture;
}

void lib3d::TriangleGroup::trim_vectors() {
  std::vector<unsigned int>(m_vertex_index).swap(m_vertex_index);
  std::vector<unsigned int>(m_texture_index).swap(m_texture_index);
  std::vector<unsigned int>(m_normal_index).swap(m_normal_index);
}