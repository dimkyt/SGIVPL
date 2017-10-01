#include "SGIVPL\LightOptionsDialog.h"

#include <QtWidgets>
#include "glm\vec3.hpp"


sgivpl::LightOptionsDialog::LightOptionsDialog()
  : QDialog(),
    m_button_box(nullptr),
    m_light_position_x(nullptr),
    m_light_position_y(nullptr),
    m_light_position_z(nullptr)
{
  setWindowTitle(tr("Light Options"));
  QVBoxLayout *layout = new QVBoxLayout();

  // Light position widgets
  QHBoxLayout *position_layout = new QHBoxLayout();
  position_layout->addWidget(new QLabel(tr("Position: ")));
  m_light_position_x = new QLineEdit();
  m_light_position_y = new QLineEdit();
  m_light_position_z = new QLineEdit();
  m_light_position_x->setMaximumWidth(40);
  m_light_position_y->setMaximumWidth(40);
  m_light_position_z->setMaximumWidth(40);
  position_layout->addWidget(m_light_position_x);
  position_layout->addWidget(m_light_position_y);
  position_layout->addWidget(m_light_position_z);

  m_button_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(m_button_box, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_button_box, SIGNAL(rejected()), this, SLOT(reject()));


  layout->addLayout(position_layout);
  layout->addWidget(m_button_box);
  setLayout(layout);
}


sgivpl::LightOptionsDialog::~LightOptionsDialog()
{
}

glm::vec3 sgivpl::LightOptionsDialog::getLightPosition() const
{
  return glm::vec3(m_light_position_x->text().toFloat(), m_light_position_y->text().toFloat(), m_light_position_z->text().toFloat());
}

void sgivpl::LightOptionsDialog::setLightPosition(glm::vec3 position)
{
  m_light_position_x->setText(QString::number(position.x));
  m_light_position_y->setText(QString::number(position.y));
  m_light_position_z->setText(QString::number(position.z));
}
