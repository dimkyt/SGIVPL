#ifndef RENDERER_H
#define RENDERER_H

// RendererGIIS
#include "RendererGIIS\Enums.h"
#include "RendererGIIS\LightSource.h"
#include "RendererGIIS\ShaderRSM.h"
#include "RendererGIIS\ShaderRenderTexture.h"
#include "RendererGIIS\ShaderLightPass.h"

// Other
#include "glm\mat4x4.hpp"

// System
#include <GL\glew.h>
#include <memory>
#include <string>
#include <vector>

namespace lib3d
{
  class Mesh;
}

namespace giis
{
  struct User
  {
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    glm::vec3 dir;
    glm::vec3 dir_strafe;
    float speed;
    float miny;
    float maxy;
  };

  struct VPL
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 contribution;

    bool isValid{ true };
  };

  class Renderer
  {
  public:
    Renderer();
    ~Renderer();

    void loadSceneFile(const std::string& filepath);

    void initialise();
    void display(RenderMode mode);

    void updateLightPosition(const glm::vec3& position);
    glm::vec3 getLightPosition() const;

  private:
    // Create required OpenGL textures
    void createTextures();

    // Set the OpenGL settings for the Reflective Shadow Map.
    void setupRSM();

    // Set the uniforms used for the RSM pass.
    // They remain unchanged for the whole pass.
    void setupRSMuniforms();

    void setupLighPassUniforms();

    // Set the OpenGL settings for the Depth Map.
    void setupDepthMap();

    // Calculate the view matrices. Called every frame.
    void calculateMatrices();

    // Render to Reflective Shadow Map target
    void renderToRSM();

    // Aggregate RSM VPLs
    void aggregateRSM();

    // Render the scene with lighting
    void lightPass();

    // Display the specified render target
    void displayRenderTarget(RenderTarget target);

    std::unique_ptr<lib3d::Mesh> m_scene;
    LightSource m_light_source;

    ShaderRSM m_shader_RSM;
    ShaderRenderTexture m_shader_render_texture;
    ShaderLightPass m_shader_light_pass;

    User m_user;

    int m_win_width;
    int m_win_height;
    int m_rsm_width;
    int m_rsm_height;
    int m_rsm_size;
    int m_depth_map_width;
    int m_depth_map_height;

    float m_rsm_near;
    float m_rsm_far;
    float m_rsm_aspect;

    std::vector<float> m_wcs_buffer;
    std::vector<float> m_normal_buffer;
    std::vector<float> m_flux_buffer;

    GLuint m_depth_map;	         // depth buffer
    GLuint m_depth_map_low;	     // RSM depth buffer
    GLuint m_wcs_map;		         // world coordinates buffer
    GLuint m_normal_map;	       // normal buffer
    GLuint m_flux_map;	         // flux buffer
    GLuint m_rsm_FBO;            // RSM Frame Buffer Object
    GLuint m_depth_FBO;          // Depth map Frame Buffer Object
    GLuint m_screen_quad_vaoId;    // Buffers for screen-sized quad
    GLuint m_screen_quad_vboId;

    glm::mat4 m_matrix_MVP_RSM;    // Modelview Projection matrix for RSM
    glm::mat4 m_matrix_MVP_user;   // Modelview Projection matrix for the user
  };
}

#endif // RENDERER_H
