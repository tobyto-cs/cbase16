#ifndef CBASE_FILE_HELPERS
#define CBASE_FILE_HELPERS

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <functional>

#include <ryml.hpp>
#include <ryml_std.hpp>
#include <fmt/core.h>

namespace fs = std::filesystem;

using std::string, std::vector, std::ifstream, std::function;

namespace cbase
{
  typedef function<void(string, string)> yaml_callback;

  void yaml_list_iterate(const fs::path& yaml_path, const yaml_callback& callback);
  ryml::Tree read_yaml_file(const fs::path& yaml_path);
}

#endif
