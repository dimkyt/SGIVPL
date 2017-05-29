#ifndef RENDERER_H
#define RENDERER_H

#include <string>

namespace giis
{
  class Renderer
  {
  public:
    Renderer();
    ~Renderer();

    void loadSceneFile(std::string filePath);

  private:
  };
}

#endif // RENDERER_H
