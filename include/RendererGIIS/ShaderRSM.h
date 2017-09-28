#ifndef SHADER_RSM_H
#define SHADER_RSM_H

#include "RendererGIIS\ShaderBase.h"

namespace giis
{
  class ShaderRSM : public ShaderBase
  {
  public:
    ShaderRSM();
    ~ShaderRSM();

    void initialise() override;

    inline GLuint getMatrixMVP() const { return m_uniform_matrix_MVP; }
    inline GLuint getHasDiffuseTexture() const { return m_has_diffuse_texture; }
    inline GLuint getDiffuseTextureSampler() const { return m_uniform_diffuse_sampler; }
    inline GLuint getDiffuseMaterial() const { return m_uniform_diffuse_material; }
    inline GLuint getLightPosition() const { return m_uniform_lightPos; }
    inline GLuint getMapCentreDirection() const { return m_uniform_map_centre_dir; }
    inline GLuint getLightIntensity() const { return m_uniform_intensity; }
    inline GLuint getMapSize() const { return m_uniform_mapSize; }
    inline GLuint getFOV() const { return m_uniform_FOV; }
    inline GLuint getAspect() const { return m_uniform_aspect; }
    inline GLuint getNearDistance() const { return m_uniform_rsm_near; }

  private:
    GLuint m_uniform_lightPos;
    GLuint m_uniform_map_centre_dir;
    GLuint m_uniform_intensity;
    GLuint m_uniform_mapSize;
    GLuint m_uniform_FOV;
    GLuint m_uniform_aspect;
    GLuint m_uniform_rsm_near;
    GLuint m_uniform_matrix_MVP;
    GLuint m_uniform_diffuse_material;
    GLuint m_uniform_diffuse_sampler;
    GLuint m_has_diffuse_texture;
  };
}

#endif // SHADER_RSM_H
