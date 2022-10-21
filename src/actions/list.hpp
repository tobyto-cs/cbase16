#ifndef CBASE_LIST_ACTION
#define CBASE_LIST_ACTION

#include "../helpers/vector_print.hpp"
#include "../scheme.h"
#include "../template.h"

namespace cbase {
  using Template = cbase::Template;
  using Scheme = cbase::Scheme;

  int list_action(const std::string& type="scheme") {
    if (type == "scheme") cbase::fancy_print(std::cout, Scheme::StringList());  
    if (type == "template") cbase::fancy_print(std::cout, Template::StringList());
    else return 1;

    return 0;
  }
}

#endif
