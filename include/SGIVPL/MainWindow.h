#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class QMenu;
class QAction;

namespace sgivpl
{
  class RenderWidget;

  class MainWindow : public QMainWindow
  {
  public:
    MainWindow();
    ~MainWindow();

    void loadFile(std::string filepath);

  private slots:
    void open();

  private:
    void createActions();
    void createMenus();

    RenderWidget* m_renderWidget;

    QMenu* m_fileMenu;
    QAction* m_openAction;

  };
}

#endif // MAINWINDOW_H
