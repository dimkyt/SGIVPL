#include "lib3d\Mesh.h"

// System includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


lib3d::Mesh::Mesh()
  : m_numVertices(0),
    m_numNormals(0),
    m_numTex(0),
    m_numGroups(0),
    m_numFaces(0),
    m_numTextures(0)
{
}

lib3d::Mesh::~Mesh()
{
  destroyBuffers();
}

void lib3d::Mesh::loadFile(const std::string & filepath)
{
  objLoader(filepath);
  indexAttributes();
  createBuffers();
}

void lib3d::Mesh::objLoader(const std::string& filepath)
{
  std::ifstream file;
  std::string line,
         type;
  bool has_norm = false,
     has_tex = false;
  unsigned int curr_mtl = 0;

  file.open(filepath);

  assert(file);

  while(getline(file, line))
  {
    type = "";
    std::istringstream stream(line);
    stream >> type;

    if(type == "mtllib")
    {
      printf("Loading materials.\n");

      std::string mtlPath = filepath.substr(0, filepath.find_last_of('\/')+1);
      std::string mtlFilename;
      stream >> mtlFilename;
      mtlLoader(mtlPath+mtlFilename);

      printf("Materials loaded!\n\n");
    }
    else if(type == "v")
    {
      glm::vec4 v;
      stream >> v.x >> v.y >> v.z; v.w = 1.0f;
      m_vertices.push_back(v);
      m_numVertices++;
      has_norm = has_tex = false;
    }
    else if(type == "vn")
    {
      glm::vec3 n;
      stream >> n.x >> n.y >> n.z;
      m_normals.push_back(n);
      m_numNormals++;
      has_norm = true;
    }
    else if(type == "vt")
    {
      glm::vec3 t;
      stream >> t.x >> t.y >> t.z;
      t.x = 1.0f-t.x;					//texture coordinate U needs to be inverted for OpenGL
      m_texcoords.push_back(t);
      m_numTex++;
      has_tex = true;
    }
    else if(type == "g")
    {
      if(!m_triangle_groups.empty())			//update previous group
      {
        m_triangle_groups.back().set_normal_flag(has_norm);
        m_triangle_groups.back().set_texture_flag(has_tex);
        m_triangle_groups.back().set_material_index(curr_mtl);
      }

      TriangleGroup g;
      m_triangle_groups.push_back(g);
      m_numGroups++;
    }
    else if(type == "usemtl")
    {
      std::string name;
      stream >> name;
      for(unsigned int i=0; i<m_numMaterials; i++)
      {
        if(name == m_materials[i].name)
        {
          //groups.back().matIdx = i;
          curr_mtl = i;
          break;
        }
      }
    }
    else if(type == "f")
    {
      glm::uvec3 vIdx,
             tIdx,
             nIdx;

      char tmp;
      glm::uvec3 tmp1 = glm::uvec3(1,1,1);

      if(has_norm && has_tex)
      {
        stream >> vIdx.x >> tmp >> tIdx.x >> tmp >> nIdx.x
               >> vIdx.y >> tmp >> tIdx.y >> tmp >> nIdx.y
               >> vIdx.z >> tmp >> tIdx.z >> tmp >> nIdx.z;

        vIdx-=tmp1; tIdx-=tmp1; nIdx-=tmp1;
      }
      else if(!has_norm && has_tex)
      {
        stream >> vIdx.x >> tmp >> tIdx.x
               >> vIdx.y >> tmp >> tIdx.y
               >> vIdx.z >> tmp >> tIdx.z;

        vIdx-=tmp1; tIdx-=tmp1;
      }
      else if(has_norm && !has_tex)
      {
        stream >> vIdx.x >> tmp >> tmp >> nIdx.x
               >> vIdx.y >> tmp >> tmp >> nIdx.y
               >> vIdx.z >> tmp >> tmp >> nIdx.z;

        vIdx-=tmp1; nIdx-=tmp1;
      }
      else if(!has_norm && !has_tex)
      {
        stream >> vIdx.x
               >> vIdx.y
               >> vIdx.z;

        vIdx-=tmp1;
      }

      TriangleGroup& t_group = m_triangle_groups.back();

      t_group.add_vertex_index(vIdx.x);
      t_group.add_vertex_index(vIdx.y);
      t_group.add_vertex_index(vIdx.z);
      t_group.expand_bounding_box(m_vertices[vIdx.x]);
      t_group.expand_bounding_box(m_vertices[vIdx.y]);
      t_group.expand_bounding_box(m_vertices[vIdx.z]);

      t_group.add_texture_index(tIdx.x);
      t_group.add_texture_index(tIdx.y);
      t_group.add_texture_index(tIdx.z);

      t_group.add_normal_index(nIdx.x);
      t_group.add_normal_index(nIdx.y);
      t_group.add_normal_index(nIdx.z);

      t_group.add_face();
      m_numFaces++;
    }
  }

  assert(file.eof());

  file.close();


  if(!m_triangle_groups.empty())					//update last group.
  {
    m_triangle_groups.back().set_normal_flag(has_norm);
    m_triangle_groups.back().set_texture_flag(has_tex);
    m_triangle_groups.back().set_material_index(curr_mtl);
  }

  //trim vectors
  for(auto triangle_group : m_triangle_groups)
  {
    triangle_group.trim_vectors();
  }
  std::vector<TriangleGroup>(m_triangle_groups).swap(m_triangle_groups);
}

void lib3d::Mesh::mtlLoader(const std::string& filepath)
{
  std::ifstream file;
  std::string line,
       type;

  file.open(filepath);

  assert(file);

  while(getline(file, line))
  {
    type = "";
    std::istringstream stream(line);
    stream>>type;

    if(type == "newmtl")
    {
      Material m;
      stream >> m.name;
      m_materials.push_back(m);
      m_numMaterials++;
    }
    else if(type == "Ka")
    {
      glm::vec3 Ka;
      stream >> Ka.x >> Ka.y >> Ka.z;
      m_materials.back().ambient = Ka;
    }
    else if(type == "Kd")
    {
      glm::vec3 Kd;
      stream >> Kd.x >> Kd.y >> Kd.z;
      m_materials.back().diffuse = Kd;
    }
    else if(type == "Ks")
    {
      glm::vec3 Ks;
      stream >> Ks.x >> Ks.y >> Ks.z;
      m_materials.back().specular = Ks;
    }
    else if(type == "Ke")
    {
      glm::vec3 Ke;
      stream >> Ke.x >> Ke.y >> Ke.z;
      m_materials.back().emission = Ke;
    }
    else if(type == "Ns")
    {
      float Ns;
      stream >> Ns;
      m_materials.back().Ns = Ns;
    }
    else if(type == "map_Ka" || type == "map_Kd" || type == "map_Ks"|| type == "map_Ke")
    {
      std::map<std::string, Texture>::iterator it;
      unsigned int mapID;
      std::string tex = "";

      if(type == "map_Ka")
        mapID = MAP_AMBIENT;
      else if(type == "map_Kd")
        mapID = MAP_DIFFUSE;
      else if(type == "map_Ks")
        mapID = MAP_SPECULAR;
      else if(type == "map_Ke")
        mapID = MAP_EMISSION;

      m_materials.back().has_texture[mapID] = true;
      stream >> tex;

      it = m_textures.find(tex);
      if(it == m_textures.end())		//Texture is not in map.
      {
        m_textures.insert(std::pair<std::string, Texture>(tex, Texture(tex)));	//Insert a new Texture object in map.

        //Get file type
        size_t tmp = tex.find(".");
        std::string tex_type = tex.substr(tmp+1, std::string::npos);

        //load texture
        bool check;
        if(tex_type != "bmp")
          check = m_textures[tex].load_tex();
        else
          check = m_textures[tex].load_BMP();

        if(check)
        {
          m_materials.back().textureID[mapID] = m_textures[tex].getID();
          m_numTextures++;
        }
        else
        {
          m_textures.erase(tex);								//in case of error delete from map
        }
      }
      else
        m_materials.back().textureID[mapID] = it->second.getID();
    }
  }

  assert(file.eof());
  file.close();

  //trim vector
  std::vector<Material>(m_materials).swap(m_materials);
}

void lib3d::Mesh::indexAttributes()
{
  std::map<std::string, unsigned int> uniques;
  std::map<std::string, unsigned int>::iterator it;
  std::string key = "";

  for(unsigned int i=0; i<m_numGroups; i++)
  {
    for(unsigned int j=0; j<m_triangle_groups[i].get_num_faces(); j++)
    {
      for(int k=0; k<3; k++)
      {
        //OBJ face format v/t/n
        unsigned int idx = 3*j+k;
        char tmp[30];
        sprintf_s(
          tmp,
          "%u%u%u",
          m_triangle_groups[i].get_vertex_index(idx), 
          m_triangle_groups[i].get_texture_index(idx), 
          m_triangle_groups[i].get_normal_index(idx)
        );
        key = tmp;
        
        it = uniques.find(key);
        if(it == uniques.end())	//vertex is not in map
        {
          m_indexed_vertices.push_back(m_vertices[m_triangle_groups[i].get_vertex_index(idx)]);
          m_indexed_texcoords.push_back(m_texcoords[m_triangle_groups[i].get_texture_index(idx)]);
          m_indexed_normals.push_back(m_normals[m_triangle_groups[i].get_normal_index(idx)]);
          unsigned int index = static_cast<unsigned int>(m_indexed_vertices.size()-1);
          indices.push_back(index);
          uniques[key] = index;
        }
        else	//vertex is in map
        {
          indices.push_back(it->second);
        }
      }
    }
  }

  //trim vector
  std::vector<unsigned int>(indices).swap(indices);
}

void lib3d::Mesh::createBuffers()
{
  GLenum ErrorCheckValue = glGetError();
     
  glGenVertexArrays(1, &m_vaoId);
  glBindVertexArray(m_vaoId);
 
  //vertex buffer
  glGenBuffers(1, &m_vboId);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
  glBufferData(GL_ARRAY_BUFFER, m_indexed_vertices.size()*sizeof(glm::vec4), &m_indexed_vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  
  //normal buffer
  glGenBuffers(1, &m_nboId);
  glBindBuffer(GL_ARRAY_BUFFER, m_nboId);
  glBufferData(GL_ARRAY_BUFFER, m_indexed_normals.size()*sizeof(glm::vec3), &m_indexed_normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //texture buffer
  glGenBuffers(1, &m_tboId);
  glBindBuffer(GL_ARRAY_BUFFER, m_tboId);
  glBufferData(GL_ARRAY_BUFFER, m_indexed_texcoords.size()*sizeof(glm::vec3), &m_indexed_texcoords[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //index buffer
  glGenBuffers(1, &m_iboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
 

  ErrorCheckValue = glGetError();
  assert(ErrorCheckValue == GL_NO_ERROR);
  if (ErrorCheckValue != GL_NO_ERROR)
  {
    fprintf(
      stderr,
      "ERROR: Could not create a buffer: %s \n",
      gluErrorString(ErrorCheckValue)
    );
 
    exit(-1);
  }

  printf("OpenGL buffers created!\n");
}

void lib3d::Mesh::destroyBuffers()
{
  glDeleteBuffers(1, &m_vboId);
  glDeleteBuffers(1, &m_nboId);
  glDeleteBuffers(1, &m_tboId);
  glDeleteBuffers(1, &m_iboId);
}

const std::vector<lib3d::TriangleGroup>& lib3d::Mesh::get_triangle_groups() const
{
  return m_triangle_groups;
}

const lib3d::Material& lib3d::Mesh::get_material(unsigned int index) const
{
  return m_materials.at(index);
}

GLuint lib3d::Mesh::get_vertex_array_object_id() const
{
  return m_vaoId;
}

GLuint lib3d::Mesh::get_indexed_buffer_object_id() const
{
  return m_iboId;
}