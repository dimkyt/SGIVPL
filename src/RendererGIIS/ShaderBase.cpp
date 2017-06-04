#include "RendererGIIS\ShaderBase.h"

giis::ShaderBase::ShaderBase()
{
}

giis::ShaderBase::~ShaderBase()
{
  glDeleteProgram(m_shaderID);
}
