#include "RendererGIIS\LightSource.h"

giis::LightSource::LightSource()
  : m_position(197.0f, 71.0f, 65.0f),
    m_up_vector(0.0f, 1.0f, 0.0f),
    m_target_vector(0.0f, 21.0f, 0.0f),
    m_intensity(50000000.0f, 50000000.0f, 50000000.0f),
    m_FOV(120.0f)
{
}

giis::LightSource::~LightSource()
{
}

void giis::LightSource::setPosition(const glm::vec3& position)
{
  m_position = position;
}

void giis::LightSource::setUpVector(const glm::vec3& up_vector)
{
  m_up_vector = up_vector;
}

void giis::LightSource::setTargetVector(const glm::vec3& target_vector)
{
  m_target_vector = target_vector;
}

void giis::LightSource::setIntensity(const glm::vec3& intensity)
{
  m_intensity = intensity;
}

void giis::LightSource::setFOV(float FOV)
{
  m_FOV = FOV;
}
