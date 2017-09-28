#pragma once
#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include "RendererGIIS\LightSource.h"

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
    Q_OBJECT

  public:
    RenderWidget();
    ~RenderWidget();

    void initializeGL() override;
    void paintGL() override;

    void loadSceneFile(const std::string& filepath);

    void updateLight(const giis::LightSource& light);

  private:
    std::unique_ptr<giis::Renderer> m_renderer;

  };
}

#endif // RENDERWIDGET_H
