#include "RendererGIIS\ShaderDrawVPLs.h"

// RendererGIIS
#include "RendererGIIS\glsl.h"

// System
#include <string>

giis::ShaderDrawVPLs::ShaderDrawVPLs()
{
}

giis::ShaderDrawVPLs::~ShaderDrawVPLs()
{
}

void giis::ShaderDrawVPLs::initialise()
{
  m_shaderID = LoadShaders(std::string("DrawVPLs.vertex").c_str(), std::string("DrawVPLs.fragment").c_str());
  m_uniform_matrix_MVP = glGetUniformLocation(m_shaderID, "matrix_MVP");
}
