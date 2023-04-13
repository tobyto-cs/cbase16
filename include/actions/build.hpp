#ifndef CBASE_BUILD_ACTION
#define CBASE_BUILD_ACTION

#include <utils/po_struct.hpp>
#include <scheme/scheme.hpp>
#include <utils/macros.hpp>

namespace cbase
{
  fs::path find_template(const string& tmp);
  int build_action(const po_struct& args);
}

#endif
