#include "RendererGIIS\ShaderLightPass.h"

// RendererGIIS
#include "RendererGIIS\glsl.h"

// System
#include <string>

giis::ShaderLightPass::ShaderLightPass()
{
}

giis::ShaderLightPass::~ShaderLightPass()
{
}

void giis::ShaderLightPass::initialise()
{
  m_shaderID = LoadShaders(std::string("LightPass.vertex").c_str(), std::string("LightPass.fragment").c_str());
  m_uniform_lightPos = glGetUniformLocation(m_shaderID, "light_pos");
  m_uniform_intensity = glGetUniformLocation(m_shaderID, "constant_intensity");
  m_uniform_matrix_MVP = glGetUniformLocation(m_shaderID, "matrix_MVP");
  m_uniform_diffuse_material = glGetUniformLocation(m_shaderID, "in_diffuse_material");
  m_has_diffuse_texture = glGetUniformLocation(m_shaderID, "in_has_diffuse_texture");
  m_uniform_diffuse_sampler = glGetUniformLocation(m_shaderID, "diffuse_sampler");
}
