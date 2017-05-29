#include "RendererGIIS\Renderer.h"

#include "lib3D\Mesh.h"

giis::Renderer::Renderer()
  : m_scene(nullptr)
{
}

giis::Renderer::~Renderer()
{
}

void giis::Renderer::loadSceneFile(const std::string& filepath)
{
  m_scene.reset(new lib3d::Mesh());
  m_scene->loadFile(filepath);
}
