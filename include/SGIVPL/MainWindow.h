#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <memory>

class QMenu;
class QAction;

namespace sgivpl
{
  class LightOptionsDialog;
  class RenderWidget;

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    MainWindow();
    ~MainWindow();

    void loadFile(const std::string& filepath);

  private slots:
    void open();
    void lightOptions();
    void acceptLightOptions();

  private:
    void createActions();
    void createMenus();

    RenderWidget* m_renderWidget;

    QMenu* m_fileMenu;
    QAction* m_openAction;

    QMenu* m_toolsMenu;
    QAction* m_lightOptionsAction;

    std::unique_ptr<LightOptionsDialog> m_light_options_dialog;
  };
}

#endif // MAINWINDOW_H
