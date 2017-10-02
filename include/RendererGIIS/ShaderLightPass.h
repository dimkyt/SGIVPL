#pragma once

#include "RendererGIIS\ShaderBase.h"

namespace giis
{
  class ShaderLightPass : public ShaderBase
  {
  public:
    ShaderLightPass();
    ~ShaderLightPass();

    void initialise() override;

    inline GLuint getMatrixMVP() const { return m_uniform_matrix_MVP; }
    inline GLuint getHasDiffuseTexture() const { return m_has_diffuse_texture; }
    inline GLuint getDiffuseTextureSampler() const { return m_uniform_diffuse_sampler; }
    inline GLuint getDiffuseMaterial() const { return m_uniform_diffuse_material; }
    inline GLuint getLightPosition() const { return m_uniform_lightPos; }
    inline GLuint getLightIntensity() const { return m_uniform_intensity; }

  private:
    GLuint m_uniform_lightPos;
    GLuint m_uniform_intensity;
    GLuint m_uniform_matrix_MVP;
    GLuint m_uniform_diffuse_material;
    GLuint m_uniform_diffuse_sampler;
    GLuint m_has_diffuse_texture;
  };
}