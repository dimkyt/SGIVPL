#pragma once
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <memory>

class QMenu;
class QAction;

namespace giis
{
  class Renderer;
}

namespace sgivpl
{
  class RenderWidget : public QOpenGLWidget
  {
  public:
    RenderWidget();
    ~RenderWidget();

    void initializeGL() override;
    void paintGL() override;

    void loadSceneFile(std::string filepath);

  private:
    std::unique_ptr<giis::Renderer> m_renderer;

  };
}

#endif // RENDERWIDGET_H
