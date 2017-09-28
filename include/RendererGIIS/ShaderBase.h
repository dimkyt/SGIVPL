#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include <GL/glew.h>

namespace giis
{
  class ShaderBase
  {
  public:
    ShaderBase();
    virtual ~ShaderBase();

    virtual void initialise() = 0;

    inline GLuint getShaderID() { return m_shaderID; }

  protected:
    GLuint m_shaderID;
  };
}

#endif // SHADER_BASE_H
