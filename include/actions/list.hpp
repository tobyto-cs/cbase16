#ifndef CBASE_LIST
#define CBASE_LIST

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

using std::string;

namespace cbase
{
  int list_schemes();
  int list_templates();
}

#endif
