#pragma once

#include "RendererGIIS\ShaderBase.h"

namespace giis
{
  class ShaderShadowPass : public ShaderBase
  {
  public:
    ShaderShadowPass();
    ~ShaderShadowPass();

    void initialise() override;

    GLuint getMatrixMVP() const { return m_uniform_matrix_MVP; }

  private:
    GLuint m_uniform_matrix_MVP;
  };
}