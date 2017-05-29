#include "lib3d\Material.h"

lib3d::Material::Material()
{
	for(int i=0; i<MAP_COUNT; i++)
	{
		has_texture[i] = false;
	}

	ambient = diffuse = specular = emission = glm::vec3(.0, .0, .0);
}

lib3d::Material::~Material()
{
}