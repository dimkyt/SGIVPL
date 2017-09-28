#include "RendererGIIS\ShaderRenderTexture.h"

// RendererGIIS
#include "RendererGIIS\glsl.h"

// System
#include <string>

giis::ShaderRenderTexture::ShaderRenderTexture()
{
}

giis::ShaderRenderTexture::~ShaderRenderTexture()
{
}

void giis::ShaderRenderTexture::initialise()
{
  m_shaderID = LoadShaders(std::string("RenderTexture.vertex").c_str(), std::string("RenderTexture.fragment").c_str());
  m_uniform_sampler_color = glGetUniformLocation(m_shaderID, "tex_color");
}
