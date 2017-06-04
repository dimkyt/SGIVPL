#ifndef GLSL_H
#define GLSL_H

#include <GL/glew.h>

namespace giis
{
  GLuint LoadShaders(const char * vertex_file, const char * fragment_file);
}

#endif