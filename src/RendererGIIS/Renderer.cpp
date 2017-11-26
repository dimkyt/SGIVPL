// RendererGIIS
#include "RendererGIIS\Renderer.h"

// Other
#include "glm\gtc\matrix_transform.hpp"
#include "lib3D\Mesh.h"

// System
#include <unordered_map>

static std::unordered_map<giis::RenderMode, giis::RenderTarget> modeToTarget =
{
  std::make_pair<giis::RenderMode, giis::RenderTarget>(giis::RenderMode::RSM_WCS, giis::RenderTarget::WCS),
  std::make_pair<giis::RenderMode, giis::RenderTarget>(giis::RenderMode::RSM_NORMAL, giis::RenderTarget::NORMAL),
  std::make_pair<giis::RenderMode, giis::RenderTarget>(giis::RenderMode::RSM_FLUX, giis::RenderTarget::FLUX),
  std::make_pair<giis::RenderMode, giis::RenderTarget>(giis::RenderMode::RSM_DEPTH, giis::RenderTarget::DEPTH_LOW)
};

static const GLfloat screen_quad_data[] =
{
  -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   1.0f,  1.0f, 0.0f
};


giis::Renderer::Renderer()
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
  m_rsm_size = m_rsm_width * m_rsm_height;
  m_rsm_aspect = (float)m_rsm_width / (float)m_rsm_height;

  m_wcs_buffer.resize(m_rsm_size*3);
  m_normal_buffer.resize(m_rsm_size*3);
  m_flux_buffer.resize(m_rsm_size*3);
  m_VPL_buffer.resize(m_rsm_size);

  // User
  m_user.eye = {0.0f, 10.0f, 27.0f};

  // Light source
  m_light_source.setPosition(glm::vec3(0.0f, 19.0f, 0.0f));
  m_light_source.setUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
  m_light_source.setTargetVector(glm::vec3(0.0f, -1.0f, 0.0f));
  m_light_source.setIntensity(glm::vec3(2.0f, 2.0f, 2.0f));
  m_light_source.setFOV(130.0f);

  createTextures();
  setupRSM();
  setupDepthMap();

  // Build shaders
  m_shader_RSM.initialise();
  m_shader_render_texture.initialise();
  m_shader_shadow_pass.initialise();
  m_shader_light_pass.initialise();
  m_shader_draw_vpls.initialise();

  // Debug draw buffers
  glGenVertexArrays(1, &m_vpl_vao);
  glGenBuffers(1, &m_vpl_vbo);
}

void giis::Renderer::createTextures()
{
  // setup depth map
  glGenTextures(1, &m_depth_map);
  glBindTexture(GL_TEXTURE_2D, m_depth_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_depth_map_width, m_depth_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  // setup wcs buffer
  glGenTextures(1, &m_wcs_map);
  glBindTexture(GL_TEXTURE_2D, m_wcs_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_rsm_width, m_rsm_height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  // setup normal buffer
  glGenTextures(1, &m_normal_map);
  glBindTexture(GL_TEXTURE_2D, m_normal_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_rsm_width, m_rsm_height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  // setup flux buffer
  glGenTextures(1, &m_flux_map);
  glBindTexture(GL_TEXTURE_2D, m_flux_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_rsm_width, m_rsm_height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  // setup RSM depth texture
  glGenTextures(1, &m_depth_map_low);
  glBindTexture(GL_TEXTURE_2D, m_depth_map_low);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_rsm_width, m_rsm_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, 0);

  // setup screen-sized quad
  glGenVertexArrays(1, &m_screen_quad_vaoId);
  glBindVertexArray(m_screen_quad_vaoId);
  glGenBuffers(1, &m_screen_quad_vboId);
  glBindBuffer(GL_ARRAY_BUFFER, m_screen_quad_vboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad_data), screen_quad_data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void giis::Renderer::setupRSM()
{
  glGenFramebuffers(1, &m_rsm_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_rsm_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_map_low, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_wcs_map, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normal_map, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_flux_map, 0);
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void giis::Renderer::setupRSMuniforms()
{
  glUniformMatrix4fv(m_shader_RSM.getMatrixMVP(), 1, GL_FALSE, &m_matrix_MVP_RSM[0][0]);
  glUniform3fv(m_shader_RSM.getLightPosition(), 1, &m_light_source.getPosition()[0]);
  glUniform3fv(m_shader_RSM.getMapCentreDirection(), 1, &m_light_source.getTargetVector()[0]);
  glUniform3fv(m_shader_RSM.getLightIntensity(), 1, &m_light_source.getIntensity()[0]);
  glUniform1f(m_shader_RSM.getMapSize(), (GLfloat)m_rsm_size);
  glUniform1f(m_shader_RSM.getSolidAnglePreCalc(), 4.0f*m_rsm_aspect*glm::pow(glm::tan(glm::radians(m_light_source.getFOV()*0.5f)), 2.0f));
  glUniform1f(m_shader_RSM.getNearDistance(), m_rsm_near);
  glUniform1i(m_shader_RSM.getDiffuseTextureSampler(), lib3d::MAP_DIFFUSE);
}

void giis::Renderer::setupLighPassUniforms()
{
  glUniformMatrix4fv(m_shader_light_pass.getMatrixMVP(), 1, GL_FALSE, &m_matrix_MVP_user[0][0]);
  glUniform3fv(m_shader_light_pass.getLightPosition(), 1, &m_light_source.getPosition()[0]);
  glUniform3fv(m_shader_light_pass.getLightIntensity(), 1, &m_light_source.getIntensity()[0]);
  glUniform1i(m_shader_light_pass.getDiffuseTextureSampler(), lib3d::MAP_DIFFUSE);
}

void giis::Renderer::setupDepthMap()
{
  glGenFramebuffers(1, &m_depth_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_depth_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_map, 0);
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void giis::Renderer::calculateMatrices()
{
  // RSM matrices
  glm::mat4 matrix_M_RSM = glm::mat4(1.0f);
  glm::mat4 matrix_V_RSM = glm::lookAt(m_light_source.getPosition(), m_light_source.getTargetVector(), m_light_source.getUpVector());
  glm::mat4 matrix_P_RSM = glm::perspective(m_light_source.getFOV(), m_rsm_aspect, m_rsm_near, m_rsm_far);
  m_matrix_MVP_RSM = matrix_P_RSM * matrix_V_RSM * matrix_M_RSM;

  // Camera matrices
  glm::mat4 matrix_M_user = glm::mat4(1.0f);
  glm::mat4 matrix_V_user = glm::lookAt(m_user.eye, glm::vec3(0.0f, 9.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 matrix_P_user = glm::perspective(70.0f, (float)m_win_width / (float)m_win_height, 1.0f, 800.0f);
  m_matrix_MVP_user = matrix_P_user * matrix_V_user * matrix_M_user;
}

void giis::Renderer::renderToRSM()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_rsm_FBO);
  glViewport(0, 0, m_rsm_width, m_rsm_height);
  GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, buffers);

  GLenum ErrorCheckValue = glGetError();
  assert(ErrorCheckValue == GL_NO_ERROR);

  glClearColor(0, 0, 0, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);


  // Set per-frame uniforms.
  glUseProgram(m_shader_RSM.getShaderID());
  setupRSMuniforms();

  glBindVertexArray(m_scene->get_vertex_array_object_id());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scene->get_indexed_buffer_object_id());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Set per-group uniforms and draw
  const std::vector<lib3d::TriangleGroup> triangle_groups = m_scene->get_triangle_groups();
  unsigned int idxOffset = 0;
  for (const lib3d::TriangleGroup triangle_group : triangle_groups)
  {
    //set diffuse material
    glm::vec3 Kd = m_scene->get_material(triangle_group.get_material_index()).diffuse;
    glUniform3fv(m_shader_RSM.getDiffuseMaterial(), 1, &Kd[0]);
    
    //check for diffuse texture
    float has_diffuse_texture = m_scene->get_material(triangle_group.get_material_index()).has_texture[lib3d::MAP_DIFFUSE];
    glUniform1f(m_shader_RSM.getHasDiffuseTexture(), has_diffuse_texture);
    
    if (has_diffuse_texture == 1.0)
    {
      glActiveTexture(GL_TEXTURE0 + lib3d::MAP_DIFFUSE);
      unsigned int diffuse_textureID = m_scene->get_material(triangle_group.get_material_index()).textureID[lib3d::MAP_DIFFUSE];
      glBindTexture(GL_TEXTURE_2D, diffuse_textureID);      
    }

    glDrawElements(GL_TRIANGLES, triangle_group.get_num_vertex_index(), GL_UNSIGNED_INT, (void*)idxOffset);
    idxOffset += triangle_group.get_num_vertex_index() * sizeof(unsigned int);
  }


  glActiveTexture(GL_TEXTURE0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void giis::Renderer::shadowPass()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map);
  glViewport(0, 0, m_depth_map_width, m_depth_map_height);

  glClearDepth(1);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);

  glUseProgram(m_shader_shadow_pass.getShaderID());
  glUniformMatrix4fv(m_shader_shadow_pass.getMatrixMVP(), 1, GL_FALSE, &m_matrix_MVP_RSM[0][0]);

  glBindVertexArray(m_scene->get_vertex_array_object_id());
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scene->get_indexed_buffer_object_id());

  const std::vector<lib3d::TriangleGroup> triangle_groups = m_scene->get_triangle_groups();
  unsigned int idxOffset = 0;
  for (const lib3d::TriangleGroup triangle_group : triangle_groups)
  {
    glDrawElements(GL_TRIANGLES, triangle_group.get_num_vertex_index(), GL_UNSIGNED_INT, (void*)idxOffset);
    idxOffset += triangle_group.get_num_vertex_index() * sizeof(unsigned int);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

  glUseProgram(0);
}

void giis::Renderer::aggregateRSM()
{
  // Copy RSM to buffers for aggregation
  glBindTexture(GL_TEXTURE_2D, m_wcs_map);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &m_wcs_buffer[0]);
  
  glBindTexture(GL_TEXTURE_2D, m_normal_map);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &m_normal_buffer[0]);
  
  glBindTexture(GL_TEXTURE_2D, m_flux_map);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &m_flux_buffer[0]);
  
  glBindTexture(GL_TEXTURE_2D, 0);


  // Populate VPL buffer
  glm::vec3 invalid_normal{ 0.0f, 0.0f, 0.0f };
  for (int i=0; i<m_rsm_size; ++i)
  {
    int index = i * 3;
    giis::VPL& vpl = m_VPL_buffer[i];
    vpl.normal = { m_normal_buffer[index], m_normal_buffer[index + 1], m_normal_buffer[index + 2] };

    if (vpl.normal == invalid_normal)
    {
      vpl.isValid = false;
      continue;
    }

    vpl.position = { m_wcs_buffer[index], m_wcs_buffer[index + 1], m_wcs_buffer[index + 2] };
    vpl.contribution = { m_flux_buffer[index], m_flux_buffer[index + 1], m_flux_buffer[index + 2] };
    vpl.num_contained = 1;
  }
}

void giis::Renderer::lightPass()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 1);
  glViewport(0, 0, m_win_width, m_win_height);

  GLenum ErrorCheckValue = glGetError();
  assert(ErrorCheckValue == GL_NO_ERROR);

  glClearColor(0, 0, 0, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);


  // Set per-frame uniforms.
  glUseProgram(m_shader_light_pass.getShaderID());
  setupLighPassUniforms();

  glBindVertexArray(m_scene->get_vertex_array_object_id());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_scene->get_indexed_buffer_object_id());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Set per-group uniforms and draw
  const std::vector<lib3d::TriangleGroup> triangle_groups = m_scene->get_triangle_groups();
  unsigned int idxOffset = 0;
  for (const lib3d::TriangleGroup triangle_group : triangle_groups)
  {
    //set diffuse material
    glm::vec3 Kd = m_scene->get_material(triangle_group.get_material_index()).diffuse;
    glUniform3fv(m_shader_light_pass.getDiffuseMaterial(), 1, &Kd[0]);

    //check for diffuse texture
    float has_diffuse_texture = m_scene->get_material(triangle_group.get_material_index()).has_texture[lib3d::MAP_DIFFUSE];
    glUniform1f(m_shader_light_pass.getHasDiffuseTexture(), has_diffuse_texture);

    if (has_diffuse_texture == 1.0)
    {
      glActiveTexture(GL_TEXTURE0 + lib3d::MAP_DIFFUSE);
      unsigned int diffuse_textureID = m_scene->get_material(triangle_group.get_material_index()).textureID[lib3d::MAP_DIFFUSE];
      glBindTexture(GL_TEXTURE_2D, diffuse_textureID);
    }

    glDrawElements(GL_TRIANGLES, triangle_group.get_num_vertex_index(), GL_UNSIGNED_INT, (void*)idxOffset);
    idxOffset += triangle_group.get_num_vertex_index() * sizeof(unsigned int);
  }


  glActiveTexture(GL_TEXTURE0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void giis::Renderer::display(RenderMode mode)
{
  calculateMatrices();
  renderToRSM();
  shadowPass();
  aggregateRSM();

  if (mode != RenderMode::NORMAL)
  {
    displayRenderTarget(modeToTarget[mode]);
  }
  else
  {
    lightPass();
  }

  if (m_draw_vpls)
  {
    drawVPLs();
  }
}

void giis::Renderer::updateLightPosition(const glm::vec3& position)
{
  m_light_source.setPosition(position);
}

glm::vec3 giis::Renderer::getLightPosition() const
{
  return m_light_source.getPosition();
}

void giis::Renderer::displayRenderTarget(RenderTarget target)
{
  GLuint tgt;
  switch (target)
  {
    case RenderTarget::WCS :
      tgt = m_wcs_map;
      break;
    case RenderTarget::NORMAL :
      tgt = m_normal_map;
      break;
    case RenderTarget::FLUX :
      tgt = m_flux_map;
      break;
    case RenderTarget::DEPTH_LOW :
      tgt = m_depth_map;
      break;
    default:
      break;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 1);
  glViewport(0, 0, m_win_width, m_win_height);

  glClearDepth(1);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  glUseProgram(m_shader_render_texture.getShaderID());
  glUniform1i(m_shader_render_texture.getColorSampler(), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tgt);

  glBindVertexArray(m_screen_quad_vaoId);
  glEnableVertexAttribArray(0);

  // Draw the screen quad
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

  glUseProgram(0);
}

void giis::Renderer::drawVPLs()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 1);
  glViewport(0, 0, m_win_width, m_win_height);

  GLenum ErrorCheckValue = glGetError();
  assert(ErrorCheckValue == GL_NO_ERROR);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // Set per-frame uniforms.
  glUseProgram(m_shader_draw_vpls.getShaderID());
  glUniformMatrix4fv(m_shader_draw_vpls.getMatrixMVP(), 1, GL_FALSE, &m_matrix_MVP_user[0][0]);

  glBindVertexArray(m_vpl_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vpl_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_wcs_buffer.size()*sizeof(float), &m_wcs_buffer[0], GL_STREAM_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(0);

  glDrawArrays(GL_POINTS, 0, m_wcs_buffer.size());

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}
