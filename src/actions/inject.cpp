#include <actions/inject.hpp>

namespace cbase
{
  int inject_config(const fs::path& cfg_fp, const string& new_cfg, const string& start, const string& end)
  {
    if (not fs::exists(cfg_fp))
      throw std::runtime_error(fmt::format("Cannot inject into file {}, it does not exist", cfg_fp.string()));
    if (fs::is_directory(cfg_fp))
      throw std::runtime_error(fmt::format("Cannot inject into directory {}", cfg_fp.string()));

    stringstream buf, backup_buf;
    ifstream file;
    file.exceptions(std::ios_base::badbit);
    file.open(cfg_fp);

    bool is_injecting = false, injected = false;
    for (string line; std::getline(file, line); )
    {
      line.push_back('\n');

      if (is_injecting && not injected) {
        buf << new_cfg;
        injected = true;
      }

      if (is_injecting && injected)
      {
        if (line.find(end) != string::npos) {
          is_injecting = false;
          buf << line;
        }
      }

      else
      {
        if (not injected && line.find(start) != string::npos) is_injecting = true;
        buf << line;
      }
    }
    file.close();

    if (is_injecting)
      throw std::runtime_error(fmt::format("File {} does not contain an end string", cfg_fp.string()));

    ofstream new_file;
    new_file.open(cfg_fp, std::ios::out | std::ios::trunc);

    new_file << buf.str();

    new_file.close();
    /* std::cout << buf.str(); */

    return 0;
  }

  int inject_action(const po_struct& args)
  {
    bool verbose = args.debug.get();


    // Grab scheme from args
    Scheme scheme(args);

    ryml::Tree tree = read_yaml_file(CBASE_CONFIG_FILE);

    ryml::ConstNodeRef root = tree.rootref();
    ryml::ConstNodeRef inject = tree["inject"];
    if (not root.has_child("inject"))
      throw std::runtime_error(fmt::format("Config file does not include inject section"));


    for (ryml::ConstNodeRef const& cfg : inject.children())
    {
      try {
        string cfg_name,
               cfg_file,
               cfg_template,
               cfg_subtemplate="default",
               cfg_callback,
               cfg_start=START_IDENTIFIER,
               cfg_end=END_IDENTIFIER;

        c4::from_chars(cfg.key(), &cfg_name);

        if (verbose) fmt::print("Working on config {}\n", cfg_name);
        // file and template are required
        if (not cfg.has_child("file"))
          throw std::runtime_error(fmt::format("Inject Config {} does not include file", cfg_name));
        if (not cfg.has_child("template"))
          throw std::runtime_error(fmt::format("Inject Config {} does not include template", cfg_name));

        c4::from_chars(cfg["file"].val(), &cfg_file);
        c4::from_chars(cfg["template"].val(), &cfg_template);

        if (verbose) fmt::print("  Grabbed file and template\n");

        // Optional parameters
        if (cfg.has_child("subtemplate") && cfg["subtemplate"].is_keyval())
          c4::from_chars(cfg["subtemplate"].val(), &cfg_subtemplate);
        if (cfg.has_child("callback") && cfg["callback"].is_keyval())
          c4::from_chars(cfg["callback"].val(), &cfg_callback);
        if (cfg.has_child("start") && cfg["start"].is_keyval())
          c4::from_chars(cfg["start"].val(), &cfg_start);
        if (cfg.has_child("end") && cfg["end"].is_keyval())
          c4::from_chars(cfg["end"].val(), &cfg_end);

        fs::path tmp = find_template(fmt::format("{}::{}", cfg_template, cfg_subtemplate));


        string applied_config = scheme.apply_template(tmp);

        inject_config(fs::path(cfg_file), applied_config, cfg_start, cfg_end);

        // Handle callback execution
        if (not cfg_callback.empty())
        {
          system(cfg_callback.c_str());
        }

      } catch (std::runtime_error& e) {
        fmt::print("Unhandled Error when Injecting\n{}\n", e.what());
      }

    }

    return 0;
  };

}
