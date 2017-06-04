#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "glm\glm.hpp"

namespace giis
{
  class LightSource
  {
  public:
    LightSource();
    ~LightSource();

    glm::vec3 getPosition() const;
    void setPosition(glm::vec3& position);

    glm::vec3 getUpVector() const;
    void setUpVector(glm::vec3& up_vector);

    glm::vec3 getTargetVector() const;
    void setTargetVector(glm::vec3& target_vector);

    glm::vec3 getIntensity() const;
    void setIntensity(glm::vec3& intensity);


  private:
    glm::vec3 m_position;
    glm::vec3 m_up_vector;
    glm::vec3 m_target_vector;
    glm::vec3 m_intensity;
  };
}
#endif // LIGHT_SOURCE_H
