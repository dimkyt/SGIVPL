// RendererGIIS
#include "RendererGIIS\Renderer.h"

// Other
#include "lib3D\Mesh.h"

giis::Renderer::Renderer()
  : m_scene(nullptr),
    m_rsm_width(128),
    m_rsm_height(128),
    m_depth_map_width(2048),
    m_depth_map_height(2048)
{
}

giis::Renderer::~Renderer()
{
}

void giis::Renderer::loadSceneFile(const std::string& filepath)
{
  m_scene.reset(new lib3d::Mesh());
  m_scene->loadFile(filepath);
}

void giis::Renderer::initialise()
{
  createTextures();
  setupRSM();
  setupDepthMap();

  // Build shaders
  shader_RSM.initialise();
}

void giis::Renderer::createTextures()
{
  glGenFramebuffers(1, &m_rsm_FBO);
  glGenFramebuffers(1, &m_depth_FBO);

  //setup depth map
  glGenTextures(1, &m_depth_map);
  glBindTexture(GL_TEXTURE_2D, m_depth_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_depth_map_width, m_depth_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  //setup wcs buffer
  glGenTextures(1, &m_wcs_map);
  glBindTexture(GL_TEXTURE_2D, m_wcs_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_rsm_width, m_rsm_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  //setup normal buffer
  glGenTextures(1, &m_normal_map);
  glBindTexture(GL_TEXTURE_2D, m_normal_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_rsm_width, m_rsm_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  //setup flux buffer
  glGenTextures(1, &m_flux_map);
  glBindTexture(GL_TEXTURE_2D, m_flux_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_rsm_width, m_rsm_height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  //setup RSM depth texture
  glGenTextures(1, &m_depth_map_low);
  glBindTexture(GL_TEXTURE_2D, m_depth_map_low);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_rsm_width, m_rsm_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void giis::Renderer::setupRSM()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_rsm_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_wcs_map, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normal_map, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_flux_map, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_map_low, 0);

  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void giis::Renderer::setupDepthMap()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_depth_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_map, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
