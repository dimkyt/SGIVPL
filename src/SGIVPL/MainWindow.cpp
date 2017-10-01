#include "SGIVPL\MainWindow.h"
#include "SGIVPL\RenderWidget.h"
#include "SGIVPL\LightOptionsDialog.h"

#include "RendererGIIS\Enums.h"
#include "RendererGIIS\LightSource.h"

#include <QtWidgets>

sgivpl::MainWindow::MainWindow()
  : QMainWindow(),
    m_renderWidget(nullptr),
    m_light_options_dialog(std::make_unique<LightOptionsDialog>())
{
  setWindowTitle(tr("SGIVPL"));
  setMinimumSize(800, 600);
  resize(1024, 768);

  m_renderWidget = new RenderWidget();
  setCentralWidget(m_renderWidget);

  createActions();
  createMenus();
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
  m_light_options_dialog->setLightPosition(m_renderWidget->getLightSourcePosition());
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
  // Open file
  m_openAction = new QAction(tr("&Open"), this);
  m_openAction->setShortcuts(QKeySequence::Open);
  connect(m_openAction, &QAction::triggered, this, &MainWindow::open);

  // View Actions
  createViewMenuActions();

  // Light options
  m_lightOptionsAction = new QAction(tr("&Light Options"), this);
  connect(m_lightOptionsAction, &QAction::triggered, this, &MainWindow::lightOptions);
  connect(m_light_options_dialog.get(), &LightOptionsDialog::accepted, this, &MainWindow::acceptLightOptions);
}

void sgivpl::MainWindow::createViewMenuActions()
{
  m_displayNormal = new QAction(tr("&Normal"), this);
  m_displayNormal->setCheckable(true);
  connect(m_displayNormal, &QAction::triggered, [this] {m_renderWidget->updateRenderMode(giis::RenderMode::NORMAL); });

  m_displayRSMwcs = new QAction(tr("&RSM Wcs"), this);
  m_displayRSMwcs->setCheckable(true);
  connect(m_displayRSMwcs, &QAction::triggered, [this] {m_renderWidget->updateRenderMode(giis::RenderMode::RSM_WCS); });

  m_displayRSMnormal = new QAction(tr("&RSM Normal"), this);
  m_displayRSMnormal->setCheckable(true);
  connect(m_displayRSMnormal, &QAction::triggered, [this] {m_renderWidget->updateRenderMode(giis::RenderMode::RSM_NORMAL); });

  m_displayRSMflux = new QAction(tr("&RSM Flux"), this);
  m_displayRSMflux->setCheckable(true);
  connect(m_displayRSMflux, &QAction::triggered, [this] {m_renderWidget->updateRenderMode(giis::RenderMode::RSM_FLUX); });

  m_displayRSMdepth = new QAction(tr("&RSM Depth"), this);
  m_displayRSMdepth->setCheckable(true);
  connect(m_displayRSMdepth, &QAction::triggered, [this] {m_renderWidget->updateRenderMode(giis::RenderMode::RSM_DEPTH); });

  m_displayModeActionGroup = new QActionGroup(this);
  m_displayModeActionGroup->addAction(m_displayNormal);
  m_displayModeActionGroup->addAction(m_displayRSMwcs);
  m_displayModeActionGroup->addAction(m_displayRSMnormal);
  m_displayModeActionGroup->addAction(m_displayRSMflux);
  m_displayModeActionGroup->addAction(m_displayRSMdepth);
  m_displayModeActionGroup->setExclusive(true);
  m_displayNormal->setChecked(true);
}

void sgivpl::MainWindow::createMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAction);

  createViewMenu();

  m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
  m_toolsMenu->addAction(m_lightOptionsAction);
}

void sgivpl::MainWindow::createViewMenu()
{
  m_viewMenu = menuBar()->addMenu(tr("&View"));
  m_viewMenu->addAction(m_displayNormal);
  m_viewMenu->addAction(m_displayRSMwcs);
  m_viewMenu->addAction(m_displayRSMnormal);
  m_viewMenu->addAction(m_displayRSMflux);
  m_viewMenu->addAction(m_displayRSMdepth);
}
