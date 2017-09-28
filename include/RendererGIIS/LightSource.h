#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "glm\vec3.hpp"

namespace giis
{
  class LightSource
  {
  public:
    LightSource();
    ~LightSource();

    inline glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3& position);

    inline glm::vec3 getUpVector() const { return m_up_vector; }
    void setUpVector(const glm::vec3& up_vector);

    inline glm::vec3 getTargetVector() const { return m_target_vector; }
    void setTargetVector(const glm::vec3& target_vector);

    inline glm::vec3 getIntensity() const { return m_intensity; }
    void setIntensity(const glm::vec3& intensity);

    inline float getFOV() const { return m_FOV; }
    void setFOV(float FOV);


  private:
    glm::vec3 m_position;
    glm::vec3 m_up_vector;
    glm::vec3 m_target_vector;
    glm::vec3 m_intensity;

    float m_FOV;
  };
}
#endif // LIGHT_SOURCE_H
