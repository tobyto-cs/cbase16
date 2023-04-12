#include <actions/list.hpp>

namespace cbase
{

  int list_schemes(const bool& verbose)
  {
    map<string, BaseStatus> schemes;

    // Firstly, iterate through the list of schemes
    if (fs::exists(CBASE_SCHEMES_LIST))
    {
      yaml_callback callback = [&schemes](string key, string val) -> void {
        (void) val;
        schemes.insert(SPair(key, BaseStatus::OFFICAL));
      };
      yaml_list_iterate(CBASE_SCHEMES_LIST, callback);
    }


    // Secondly, iterate through the scheme directory
    for (const auto& dir : fs::directory_iterator(CBASE_SCHEMES_DIR))
    {
      string scheme_name = dir.path().filename();

      if (not fs::is_directory(dir.path())) continue;

      for (const auto& file : fs::directory_iterator(dir.path()))
      {
        if (file.path().extension() == ".yaml")
        {
          // Pair of iterator, and bool
          auto res = schemes.insert(SPair(scheme_name, BaseStatus::INSTALLED));
          // If scheme already exists
          if (not res.second) { res.first->second |= BaseStatus::INSTALLED; }
          break;
        }
        // TODO: Set update flag here
      }
    }

    // Finaly print out completed list
    for (SPair s : schemes)
    {
      fmt::print("{}:\n", s.first);
      BaseStatus flags = s.second;
      fmt::print("  Installed: {}\n", flagts(test(flags, BaseStatus::INSTALLED)));
      if (verbose) fmt::print(" - {}\n", test(flags, BaseStatus::OFFICAL) ? "Offical" : "Unofficial");
    }
    return 0;
  }

  bool validate_template(const fs::path& fp)
  {
    // Takes fp, reads config.yaml (if exisits), verifies all files in config exist
    if (not fs::exists(fp / "config.yaml")) return false;

    ryml::Tree tree = read_yaml_file(fp / "config.yaml");

    string key;
    for (ryml::NodeRef n : tree.rootref().children())
    {
      c4::from_chars(n.key(), &key);
      if (not fs::exists(fp / fmt::format("{}.mustache", key)))
        return false;
    }
    return true;
  }

  vector<string> get_subtemplates(const fs::path& fp)
  {
    if (not validate_template(fp))
      throw new std::runtime_error(fmt::format("Cannot get subtemplates from template {}", fp.string()));

    ryml::Tree tree = read_yaml_file(fp / "config.yaml");

    string key;
    vector<string> res;
    for (ryml::NodeRef n : tree.rootref().children())
    {
      c4::from_chars(n.key(), &key);
      res.push_back(key);
    }
    return res;
  }

  int list_templates(const bool& verbose)
  {
    map<string, TData> templates;

    // First iterate through template list
    if (fs::exists(CBASE_TEMPLATES_LIST))
    {
      yaml_callback callback = [&templates](string key, string val) -> void {
        (void) val;
        templates.insert(TPair(key, { BaseStatus::OFFICAL, vector<string>() }));
      };
      yaml_list_iterate(CBASE_TEMPLATES_LIST, callback);
    }
    // Second, iterate through installed templates

    vector<string> st;
    for (auto dir : fs::directory_iterator(CBASE_TEMPLATES_DIR))
    {
      if (not fs::is_directory(dir)) continue;

      string template_name = dir.path().filename();

      if (not fs::exists(dir.path() / "templates") || not validate_template(dir.path() / "templates"))
      {
        // Pair of iterator, and bool
        auto res = templates.insert(TPair(template_name, { BaseStatus::INSTALLED | BaseStatus::INVALID, vector<string>() }));
        // If scheme already exists
        if (not res.second) res.first->second.flags |= BaseStatus::INVALID;
      }

      else
      {
        st = get_subtemplates(dir.path() / "templates");
        auto res = templates.insert(TPair(template_name, { BaseStatus::INSTALLED, st }));
        if (not res.second) {
          res.first->second.subtemplates = st;
          res.first->second.flags |= BaseStatus::INSTALLED;
        }
      }
    }

    for (TPair t : templates)
    {
      BaseStatus flags = t.second.flags;
      fmt::print("{}:\n", t.first);
      fmt::print("  Installed: {}\n", flagts(test(flags, BaseStatus::INSTALLED)));
      fmt::print("{}", test(flags, BaseStatus::INVALID) ? "  Invalid Installation\n" : "");
      fmt::print("  Subtemplates:\n");
      for (string st : t.second.subtemplates)
        fmt::print("   - {}\n", st);
    }

    (void)verbose;
    return 0;
  }
}
