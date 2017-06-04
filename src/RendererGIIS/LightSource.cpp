#include "RendererGIIS\LightSource.h"

giis::LightSource::LightSource()
  : m_position(197.0f, 71.0f, 65.0f),
    m_up_vector(0.0f, 1.0f, 0.0f),
    m_target_vector(0.0f, 21.0f, 0.0f),
    m_intensity(50000000.0f, 50000000.0f, 50000000.0f)
{
}

giis::LightSource::~LightSource()
{
}

glm::vec3 giis::LightSource::getPosition() const
{
  return m_position;
}

void giis::LightSource::setPosition(glm::vec3& position)
{
  m_position = position;
}

glm::vec3 giis::LightSource::getUpVector() const
{
  return glm::vec3();
}

void giis::LightSource::setUpVector(glm::vec3& up_vector)
{
  m_up_vector = up_vector;
}

glm::vec3 giis::LightSource::getTargetVector() const
{
  return glm::vec3();
}

void giis::LightSource::setTargetVector(glm::vec3& target_vector)
{
  m_target_vector = target_vector;
}

glm::vec3 giis::LightSource::getIntensity() const
{
  return glm::vec3();
}

void giis::LightSource::setIntensity(glm::vec3& intensity)
{
  m_intensity = intensity;
}
