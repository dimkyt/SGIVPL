#ifndef _Texture_
#define _Texture_

#include <glm\glm.hpp>
#include <string>

namespace lib3d
{
  class Texture
  {
  public:
    Texture();
    Texture(std::string fname);
    ~Texture(void);

    std::string getFilename() { return filename; }
    unsigned int getID() { return ID; }
    bool load_BMP();
    bool load_tex();

  private:
    std::string filename;
    unsigned int ID;
  };
}

#endif
