#include "RendererGIIS\ShaderShadowPass.h"

// RendererGIIS
#include "RendererGIIS\glsl.h"

// System
#include <string>

giis::ShaderShadowPass::ShaderShadowPass()
{
}

giis::ShaderShadowPass::~ShaderShadowPass()
{
}

void giis::ShaderShadowPass::initialise()
{
  m_shaderID = LoadShaders(std::string("ShadowPass.vertex").c_str(), std::string("ShadowPass.fragment").c_str());
  m_uniform_matrix_MVP = glGetUniformLocation(m_shaderID, "matrix_MVP");
}
