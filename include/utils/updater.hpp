#ifndef CBASE_UPDATER
#define CBASE_UPDATER

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>

#include <curl/curl.h>

extern "C" {
#include <git2.h>
}

#include <ryml.hpp>
#include <ryml_std.hpp>
#include <fmt/core.h>

#include <utils/macros.hpp>

#include <iostream>

namespace fs = std::filesystem;

using std::string, std::ifstream, std::vector, std::cout;

// Used to:
//    retrieve "offical" base16 schemes/templates
//    initally setup configuration file structure (with default files)
//      could be part of installation?
namespace cbase
{

  // only checks if there is updates avalable
  bool check_updates(const bool& debug=false);

  // check and run needed updates
  bool do_updates(const bool& debug=false);
  bool list_git(const fs::path& dir, const bool& debug=false);
  inline bool update_templates() { return list_git(CBASE_TEMPLATES_DIR); }
  inline bool update_schemes() { return list_git(CBASE_SCHEMES_DIR); }

  // Checks what is missing and retrieves needed files
  int validate_config(const bool& debug=false);

  // Check if dir exists, if not then create it
  bool exist_create_dir(const fs::path& fp);

  bool download_from_url(const string& url, const fs::path& out);

  bool handle_curl_res(const CURLcode& res, const string& errstr="");

}

#endif
