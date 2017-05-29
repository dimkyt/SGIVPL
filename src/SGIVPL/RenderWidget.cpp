#include "gl\glew.h"

// SGIVPL
#include "SGIVPL\RenderWidget.h"

// Other
#include "RendererGIIS\Renderer.h"

// System
#include <cassert>

sgivpl::RenderWidget::RenderWidget()
  : QOpenGLWidget(),
    m_renderer(nullptr)
{
}

sgivpl::RenderWidget::~RenderWidget()
{
}

void sgivpl::RenderWidget::initializeGL()
{
  GLenum GlewInitResult;

  glewExperimental = GL_TRUE;
  GlewInitResult = glewInit();

  assert(GlewInitResult == GLEW_OK);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void sgivpl::RenderWidget::paintGL()
{
}

void sgivpl::RenderWidget::loadSceneFile(std::string filepath)
{
  m_renderer.reset(new giis::Renderer());
  m_renderer->loadSceneFile(filepath);
}
