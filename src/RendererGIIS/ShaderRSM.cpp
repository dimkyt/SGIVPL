#include "RendererGIIS\ShaderRSM.h"

// RendererGIIS
#include "RendererGIIS\glsl.h"

// System
#include <string>

giis::ShaderRSM::ShaderRSM()
{
}

giis::ShaderRSM::~ShaderRSM()
{
}

void giis::ShaderRSM::initialise()
{
  m_shaderID = LoadShaders(std::string("RSM_pass.vertex").c_str(), std::string("RSM_pass.fragment").c_str());
  m_uniform_lightPos = glGetUniformLocation(m_shaderID, "light_pos");
  m_uniform_map_centre_dir = glGetUniformLocation(m_shaderID, "map_centre_dir");
  m_uniform_intensity = glGetUniformLocation(m_shaderID, "constant_intensity");
  m_uniform_mapSize = glGetUniformLocation(m_shaderID, "map_size");
  m_uniform_FOV = glGetUniformLocation(m_shaderID, "FOV");
  m_uniform_aspect = glGetUniformLocation(m_shaderID, "aspect");
  m_uniform_rsm_near = glGetUniformLocation(m_shaderID, "rsm_near");
  m_uniform_matrix_MVP = glGetUniformLocation(m_shaderID, "matrix_MVP");
  m_uniform_diffuse_material = glGetUniformLocation(m_shaderID, "in_diffuse_material");
  m_has_diffuse_texture = glGetUniformLocation(m_shaderID, "in_has_diffuse_texture");
  m_uniform_diffuse_sampler = glGetUniformLocation(m_shaderID, "diffuse_sampler");
}
