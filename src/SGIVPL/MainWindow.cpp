#include "SGIVPL\MainWindow.h"
#include "SGIVPL\RenderWidget.h"

#include <QtWidgets>

sgivpl::MainWindow::MainWindow()
  : QMainWindow(),
    m_renderWidget(nullptr)
{
  createActions();
  createMenus();

  setWindowTitle(tr("Menus"));
  setMinimumSize(800, 600);
  resize(1024, 768);

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);
}

sgivpl::MainWindow::~MainWindow()
{
}

void sgivpl::MainWindow::loadFile(std::string filepath)
{
  m_renderWidget->loadSceneFile(filepath);
}

void sgivpl::MainWindow::open()
{
  std::string filepath = QFileDialog::getOpenFileName(
    this,
    tr("Open File"),
    "",
    tr("Files (*.obj)")
  ).toStdString();

  loadFile(filepath);
}

void sgivpl::MainWindow::createActions()
{
  m_openAction = new QAction(tr("&Open"), this);
  m_openAction->setShortcuts(QKeySequence::Open);
  connect(m_openAction, &QAction::triggered, this, &MainWindow::open);
}

void sgivpl::MainWindow::createMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAction);
}
