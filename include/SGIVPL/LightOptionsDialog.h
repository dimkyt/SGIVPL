#ifndef LIGHT_OPTIONS_DIALOG_H
#define LIGHT_OPTIONS_DIALOG_H

#include "RendererGIIS\LightSource.h"

#include <QDialog>
#include "glm\vec3.hpp"

class QDialogButtonBox;
class QLineEdit;

namespace sgivpl
{
  class LightOptionsDialog : public QDialog
  {
    Q_OBJECT

  public:
    LightOptionsDialog();
    ~LightOptionsDialog();

    glm::vec3 getLightPosition() const;
    void setLightPosition(glm::vec3 position);

  private:
    QLineEdit* m_light_position_x;
    QLineEdit* m_light_position_y;
    QLineEdit* m_light_position_z;

    QDialogButtonBox* m_button_box;
  };
}

#endif // LIGHT_OPTIONS_DIALOG_H