#include <utils/file_helpers.hpp>

namespace cbase
{
  // Assumes simple key/val yaml file
  void yaml_list_iterate(const fs::path& yaml_path, const yaml_callback& callback)
  {
    ifstream file;
    file.exceptions(std::ios_base::badbit);
    file.open(yaml_path);
    string list_buf((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(list_buf));

    string key, val;
    for (ryml::NodeRef n : tree.rootref().children())
    {
      c4::from_chars(n.key(), &key);
      c4::from_chars(n.val(), &val);

      // Callback takes two strings, does something, then returns nothing
      callback(key, val);
    }
  }

  ryml::Tree read_yaml_file(const fs::path& yaml_path)
  {
    if (not fs::exists(yaml_path) || yaml_path.extension() != ".yaml")
      throw std::runtime_error(fmt::format("Cannot read yaml from {}", yaml_path.string()));

    ifstream file;
    file.exceptions(std::ios_base::badbit);
    file.open(yaml_path);
    string list_buf((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    return ryml::parse_in_arena(ryml::to_csubstr(list_buf));
  }
}
