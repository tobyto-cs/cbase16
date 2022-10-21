#include "cbase_path.hpp"

namespace cbase {
  void schemedir_parser(const fs::path fp, const std::function<void(fs::path)> dir_func) {
    for (const auto & schemedir : fs::directory_iterator(fp))
      for (const auto & filepath : fs:: directory_iterator(schemedir)) {
        dir_func(filepath.path()); 
      }
  }

  void templatedir_parser(const fs::path fp, const std::function<void(fs::path)> dir_func) {
    for (const auto& template_path : fs::directory_iterator(fp))
      dir_func(template_path.path());
  }

  fs::path fp_checker(const std::string& fp, const std::string& type) {
    fs::path search_path;
    if (!fp.empty()) search_path = fs::path(fp); 
    else search_path = fs::path(CONFIG_DIR) / type;
    assert(fs::exists(search_path));
    return search_path;
  }

  fs::path schemefp_checker(const std::string& fp) { return fp_checker(fp, "schemes"); }
  fs::path templatefp_checker(const std::string& fp) { return fp_checker(fp, "templates"); }
}
