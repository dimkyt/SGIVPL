#ifndef RENDERER_H
#define RENDERER_H

// RendererGIIS
#include "RendererGIIS\LightSource.h"
#include "RendererGIIS\ShaderRSM.h"

// Other
#include "glm\mat4x4.hpp"

// System
#include <GL\glew.h>
#include <memory>
#include <string>

namespace lib3d
{
  class Mesh;
}

namespace giis
{
  class Renderer
  {
  public:
    Renderer();
    ~Renderer();

    void loadSceneFile(const std::string& filepath);

    void initialise();
    void display();

  private:
    // Create required OpenGL textures
    void createTextures();

    // Set the OpenGL settings for the Reflective Shadow Map.
    void setupRSM();

    // Set the OpenGL settings for the Depth Map.
    void setupDepthMap();

    // Calculate the view matrices. Called every frame.
    void calculateMatrices();

    // Render to Reflective Shadow Map target
    void renderToRSM();

    // Display the specified render target
    void displayComponent();

    std::unique_ptr<lib3d::Mesh> m_scene;
    LightSource m_light_source;

    ShaderRSM m_shader_RSM;

    int m_rsm_width;
    int m_rsm_height;
    int m_rsm_size;
    int m_depth_map_width;
    int m_depth_map_height;

    float m_rsm_near;
    float m_rsm_far;
    float m_rsm_aspect;

    GLuint m_depth_map;	         // depth buffer
    GLuint m_depth_map_low;	     // RSM depth buffer
    GLuint m_wcs_map;		         // world coordinates buffer
    GLuint m_normal_map;	       // normal buffer
    GLuint m_flux_map;	         // flux buffer
    GLuint m_rsm_FBO;            // RSM Frame Buffer Object
    GLuint m_depth_FBO;          // Depth map Frame Buffer Object

    glm::mat4 m_matrix_M_RSM;      // Model matrix for RSM
    glm::mat4 m_matrix_V_RSM;      // View matrix for RSM
    glm::mat4 m_matrix_P_RSM;      // Projection matrix for RSM
    glm::mat4 m_matrix_MVP_RSM;    // Modelview Projection matrix for RSM
  };
}

#endif // RENDERER_H
