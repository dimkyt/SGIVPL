#ifndef SHADER_RENDER_TEXTURE_H
#define SHADER_RENDER_TEXTURE_H

#include "RendererGIIS\ShaderBase.h"

namespace giis
{
  class ShaderRenderTexture : public ShaderBase
  {
  public:
    ShaderRenderTexture();
    ~ShaderRenderTexture();

    void initialise() override;

    inline GLuint getColorSampler() const { return m_uniform_sampler_color; }

  private:
    GLuint m_uniform_sampler_color;
  };
}

#endif // SHADER_RENDER_TEXTURE_H