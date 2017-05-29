/*!
 * \class Mesh
 *
 * Contains all the mesh data (vertices, faces, normals etc).
 * Also creates the necessary openGL buffers.
 * Binds attributes 0/1/2 to vertices/normals/textures
 *
 * \author Dimitrios
 */



#ifndef _Mesh_
#define _Mesh_

#include "lib3d\TriangleGroup.h"
#include "lib3d\Material.h"
#include "lib3d\Texture.h"

// System includes
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <map>

namespace lib3d
{
  class Mesh
  {

  public:

    // Constructor
    Mesh();

    // Destructor
    ~Mesh();

    void loadFile(const std::string& filepath);
    void objLoader(const std::string& filepath);
    void mtlLoader(const std::string& filepath);


    //align attributes for indexed drawing
    void indexAttributes();

    // Create the openGL buffers
    // Reserves attributes 0/1/2 for vertex/normal/texture
    void createBuffers();

    void destroyBuffers();

    // Returns the triangle groups of the Mesh.
    const std::vector<TriangleGroup>& get_triangle_groups() const;

    // Returns the material at the given index.
    const Material& get_material(unsigned int index) const;

    // Returns the vaoID.
    GLuint get_vertex_array_object_id() const;

    // Returns the iboID.
    GLuint get_indexed_buffer_object_id() const;

  private:
    std::vector<glm::vec4> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_texcoords;
    std::vector<TriangleGroup> m_triangle_groups;
    std::vector<Material> m_materials;
    std::map<std::string, Texture> m_textures;

    //Used for indexed rendering
    std::vector<glm::vec4> m_indexed_vertices;
    std::vector<glm::vec3> m_indexed_normals;
    std::vector<glm::vec3> m_indexed_texcoords;

    std::vector<unsigned int> indices;

    GLuint m_vaoId;
    GLuint m_vboId;
    GLuint m_nboId;
    GLuint m_tboId;
    GLuint m_iboId;

    unsigned int m_numVertices;
    unsigned int m_numNormals;
    unsigned int m_numTex;
    unsigned int m_numGroups;
    unsigned int m_numFaces;
    unsigned int m_numMaterials;
    unsigned int m_numTextures;
  };
}
#endif
