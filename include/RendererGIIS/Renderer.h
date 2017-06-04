#ifndef RENDERER_H
#define RENDERER_H

// RendererGIIS
#include "RendererGIIS\LightSource.h"
#include "RendererGIIS\ShaderRSM.h"

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

  private:
    // Create required OpenGL textures
    void createTextures();


    // Set the OpenGL settings for the Reflective Shadow Map.
    void setupRSM();

    // Set the OpenGL settings for the Depth Map.
    void setupDepthMap();

    std::unique_ptr<lib3d::Mesh> m_scene;
    LightSource m_light_source;

    ShaderRSM shader_RSM;

    int m_rsm_width;
    int m_rsm_height;
    int m_depth_map_width;
    int m_depth_map_height;

    GLuint m_depth_map;	         // depth buffer
    GLuint m_depth_map_low;	     // RSM depth buffer
    GLuint m_wcs_map;		         // world coordinates buffer
    GLuint m_normal_map;	       // normal buffer
    GLuint m_flux_map;	         // flux buffer
    GLuint m_rsm_FBO;            // RSM Frame Buffer Object
    GLuint m_depth_FBO;
  };
}

#endif // RENDERER_H
