#include "cbase_path.hpp"

namespace cbase {
  void dir_parser(const fs::path fp, const std::function<void(fs::path)> dir_func) {
    for (const auto & schemedir : fs::directory_iterator(fp))
      for (const auto & filepath : fs:: directory_iterator(schemedir)) {
        dir_func(filepath.path()); 
      }
  }

  fs::path fp_checker(const std::string& fp) {
    fs::path schemesPath;
    if (!fp.empty()) schemesPath = fs::path(fp);
    else schemesPath = fs::path(CONFIG_DIR) / "schemes";

    // TODO: turn into an exception
    assert(fs::exists(schemesPath));

    return schemesPath;
  }
}
