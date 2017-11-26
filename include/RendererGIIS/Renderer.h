#ifndef RENDERER_H
#define RENDERER_H

// RendererGIIS
#include "RendererGIIS\Enums.h"
#include "RendererGIIS\LightSource.h"
#include "RendererGIIS\ShaderDrawVPLs.h"
#include "RendererGIIS\ShaderLightPass.h"
#include "RendererGIIS\ShaderRenderTexture.h"
#include "RendererGIIS\ShaderRSM.h"
#include "RendererGIIS\ShaderShadowPass.h"

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
    unsigned num_contained{ 0 };
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

    // Generate high resolution shadow map
    void shadowPass();

    // Aggregate RSM VPLs
    void aggregateRSM();

    // Render the scene with lighting
    void lightPass();

    // Display the specified render target
    void displayRenderTarget(RenderTarget target);

    // Draw the VPLs. For Debug purposes.
    void drawVPLs();

    std::unique_ptr<lib3d::Mesh> m_scene;
    LightSource m_light_source;

    ShaderRSM m_shader_RSM;
    ShaderRenderTexture m_shader_render_texture;
    ShaderShadowPass m_shader_shadow_pass;
    ShaderLightPass m_shader_light_pass;
    ShaderDrawVPLs m_shader_draw_vpls;

    User m_user;

    int m_win_width {1024};
    int m_win_height {768};
    int m_rsm_width {128};
    int m_rsm_height {128};
    int m_rsm_size {0};
    int m_depth_map_width {2048};
    int m_depth_map_height {2048};

    float m_rsm_near {1.0f};
    float m_rsm_far {800.0f};
    float m_rsm_aspect {1.0f};

    std::vector<float> m_wcs_buffer;
    std::vector<float> m_normal_buffer;
    std::vector<float> m_flux_buffer;
    std::vector<VPL> m_VPL_buffer;

    GLuint m_depth_map;	         // depth buffer
    GLuint m_depth_map_low;	     // RSM depth buffer
    GLuint m_wcs_map;		         // world coordinates buffer
    GLuint m_normal_map;	       // normal buffer
    GLuint m_flux_map;	         // flux buffer
    GLuint m_rsm_FBO;            // RSM Frame Buffer Object
    GLuint m_depth_FBO;          // Depth map Frame Buffer Object
    GLuint m_screen_quad_vaoId;    // Buffers for screen-sized quad
    GLuint m_screen_quad_vboId;

    // Used for Debug draw
    bool m_draw_vpls {false};
    GLuint m_vpl_vao;
    GLuint m_vpl_vbo;

    glm::mat4 m_matrix_MVP_RSM;    // Modelview Projection matrix for RSM
    glm::mat4 m_matrix_MVP_user;   // Modelview Projection matrix for the user
  };
}

#endif // RENDERER_H
