#include "ftp/util.hpp"

namespace ftp { namespace util {

void LFtoCRLF(const char* source, size_t len, std::vector<char>& dest)
{
  dest.reserve(len * 2);
  dest.clear();
  
  for (size_t i = 0; i < len; ++i)
  {
    if (source[i] == '\n' && i != 0 && source[i - 1] != '\r') dest.push_back('\r');
    dest.push_back(source[i]);
  }
}

void CRLFtoLF(const char* source, size_t len, std::vector<char>& dest)
{
  dest.reserve(len);
  dest.clear();
  
  for (size_t i = 0; i < len; ++i)
  {
    if (source[i] != '\r') dest.push_back(source[i]);
  }
}

} /* util namespace */
} /* ftp namespace */
