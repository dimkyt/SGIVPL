#include "SGIVPL\MainWindow.h"
#include "SGIVPL\RenderWidget.h"
#include "SGIVPL\LightOptionsDialog.h"

#include "RendererGIIS\LightSource.h"

#include <QtWidgets>

sgivpl::MainWindow::MainWindow()
  : QMainWindow(),
    m_renderWidget(nullptr),
    m_light_options_dialog(std::make_unique<LightOptionsDialog>())
{
  createActions();
  createMenus();

  setWindowTitle(tr("SGIVPL"));
  setMinimumSize(800, 600);
  resize(1024, 768);

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);
}

sgivpl::MainWindow::~MainWindow()
{
}

void sgivpl::MainWindow::loadFile(const std::string& filepath)
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

  if (filepath.size() > 0)
  {
    loadFile(filepath);
  }
}

void sgivpl::MainWindow::lightOptions()
{
  connect(m_light_options_dialog.get(), SIGNAL(accepted()), this, SLOT(acceptLightOptions()));
  m_light_options_dialog->exec();
}

void sgivpl::MainWindow::acceptLightOptions()
{
  giis::LightSource light;
  LightOptionsDialog* dlg = qobject_cast<LightOptionsDialog*>(sender());
  light.setPosition(dlg->getLightPosition());

  m_renderWidget->updateLight(light);
}

void sgivpl::MainWindow::createActions()
{
  m_openAction = new QAction(tr("&Open"), this);
  m_openAction->setShortcuts(QKeySequence::Open);
  connect(m_openAction, &QAction::triggered, this, &MainWindow::open);

  m_lightOptionsAction = new QAction(tr("&Light Options"), this);
  connect(m_lightOptionsAction, &QAction::triggered, this, &MainWindow::lightOptions);
}

void sgivpl::MainWindow::createMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAction);

  m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
  m_toolsMenu->addAction(m_lightOptionsAction);
}
