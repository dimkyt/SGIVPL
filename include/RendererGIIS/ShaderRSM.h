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

    void initialise();

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
    GLuint m_uniform_colour_sampler;
    GLuint m_uniform_diffuse_sampler;
    GLuint m_has_colour_texture;
    GLuint m_has_diffuse_texture;
  };
}

#endif // SHADER_RSM_H
