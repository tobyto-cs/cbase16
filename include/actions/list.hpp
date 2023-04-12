#ifndef CBASE_LIST
#define CBASE_LIST

#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

#include <utils/macros.hpp>
#include <utils/file_helpers.hpp>
#include <ryml.hpp>
#include <fmt/core.h>

namespace fs = std::filesystem;

using std::string, std::map, std::ifstream, std::vector, std::pair;

namespace cbase
{
  inline string flagts(const bool& b) {
    return b ? "✓" : "X";
  }

  enum class BaseStatus : std::uint8_t {
    DEFAULT   = 0,
    INSTALLED = 1 << 0, // Active if installed
    OFFICAL   = 1 << 1, // From the offical lists
    UPDATES   = 1 << 2, // Active if git update is
    INVALID   = 1 << 3,
  };

  struct TData {
    BaseStatus flags;
    vector<string> subtemplates;
  };

  inline BaseStatus operator|(BaseStatus lhs, BaseStatus rhs)
  {
    return static_cast<BaseStatus>(
        static_cast<std::underlying_type_t<BaseStatus>>(lhs) |
        static_cast<std::underlying_type_t<BaseStatus>>(rhs)
    );
  }
  inline BaseStatus operator&(BaseStatus lhs, BaseStatus rhs)
  {
    return static_cast<BaseStatus>(
        static_cast<std::underlying_type_t<BaseStatus>>(lhs) &
        static_cast<std::underlying_type_t<BaseStatus>>(rhs)
    );
  }
  // https://stackoverflow.com/questions/15889414/how-to-overload-operator-on-scoped-enum
  inline BaseStatus& operator|=(BaseStatus& a, BaseStatus b)
  {
    return a = a | b;
  }

  inline bool operator!(BaseStatus f) { return f == BaseStatus::DEFAULT; }

  inline bool test(BaseStatus f, BaseStatus mask) {
    return !!(f & mask);
  }

  // Use binary flags for statuses
  //   - Installed
  //   - External
  //   - Needs Updates
  typedef pair<string, BaseStatus> SPair;
  typedef pair<string, TData> TPair;

  int list_schemes(const bool& verbose = false);
  int list_templates(const bool& verbose = false);
}

#endif
