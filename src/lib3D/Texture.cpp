
#include "lib3d\Texture.h"
#include "SOIL\src\SOIL.h"

//System includes
#include <GL\glew.h>
#include <vector>

using namespace lib3d;

Texture::Texture()
{
}

Texture::Texture(std::string fname)
  : filename(fname)
{
}

Texture::~Texture(void)
{
  glDeleteTextures(1, &ID);
}

bool Texture::load_BMP()
{
  FILE* file;
  unsigned char header[54];		//BMP header is 54 bytes
  unsigned int img_width;
  unsigned int img_height;
  unsigned int img_size;
  std::vector<unsigned char> img_data;

  //Load a BMP file
  file = fopen(filename.c_str(), "rb");

  if(!file)
  {
    printf("Image could not be opened.\n");
    return false;
  }

  if( fread(header, 1, 54, file) != 54 )		//wrong header size
  {
    printf("Not a valid BMP file.\n");
    return false;
  }

  if ( header[0]!='B' || header[1]!='M' )		//not a BMP
  {
    printf("Not a valid BMP file.\n");
    return false;
  }

  img_width = *(int*)&header[18];
  img_height = *(int*)&header[22];
  img_size = *(int*)&header[34];
  img_data.resize(img_size);
  int comp = *(int*)&header[28];

  fread(&img_data[0], 1, img_size, file);		//read image data
  fclose(file);


  //Create the texture
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_BGR, GL_UNSIGNED_BYTE, &img_data[0]);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}


bool Texture::load_tex()
{
  int img_width;
  int img_height;
  int img_channels;

  //Load file
  unsigned char* img_data = SOIL_load_image(filename.c_str(), &img_width, &img_height, &img_channels, SOIL_LOAD_AUTO);
  if(img_data == 0)
  {
    printf("File %s could not be loaded.\n", filename.c_str());
    return false;
  }


  //Create the texture
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  SOIL_free_image_data(img_data);

  return true;
}