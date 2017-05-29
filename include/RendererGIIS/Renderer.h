#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <string>

namespace lib3d
{
  class Mesh;
}

namespace giis
{
  class Renderer
  {
  public:
    Renderer();
    ~Renderer();

    void loadSceneFile(const std::string& filepath);

  private:
    std::unique_ptr<lib3d::Mesh> m_scene;
  };
}

#endif // RENDERER_H
