#ifndef CBASE_SCHEME
#define CBASE_SCHEME

#include <filesystem>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include <utils/po_struct.hpp>
#include <utils/Color.hpp>
#include <utils/file_helpers.hpp>
#include <utils/macros.hpp>

#include <ryml.hpp>
#include <ryml_std.hpp>

#include <mstch/mstch.hpp>

#include <fmt/core.h>

namespace fs = std::filesystem;

using cbase::po_struct;
using std::set, std::string, std::vector, std::map, std::error_code,
      std::ifstream, std::stringstream, std::pair;

namespace cbase {
  class Scheme
  {
    private:
      static const set<string> validTags;
      static bool is_valid_tag(const std::string tag) { return validTags.find(tag) != validTags.end(); }

      map<string, string> tags;

      mstch::map build_map()&;

    public:
      Scheme(const po_struct& args);

      // Returns a VALID tag
      const string get_tag(const string& tag);

      // Reads template from file (or from buffer)
      const string apply_template(const fs::path& fp);
      const string apply_template(const string& buf);
  };
}

#endif
