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
  QSurfaceFormat format;
  format.setMajorVersion(4);
  format.setMinorVersion(5);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  setFormat(format);
  setUpdateBehavior(UpdateBehavior::NoPartialUpdate);
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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void sgivpl::RenderWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (m_renderer)
  {
    m_renderer->display();
  }
}

void sgivpl::RenderWidget::loadSceneFile(const std::string& filepath)
{
  makeCurrent();
  m_renderer.reset(new giis::Renderer());
  m_renderer->loadSceneFile(filepath);
  m_renderer->initialise();
}
