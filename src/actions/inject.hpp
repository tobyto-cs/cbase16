#ifndef CBASE_INJECT
#define CBASE_INJECT


// ryml headers
#include <ryml.hpp>
#include <ryml_std.hpp>

// cbase headers
#include "../scheme.h"
#include "../template.h"
#include "build.hpp"
#include "../helpers/macros.h"

namespace fs = std::filesystem;
namespace cbase {
  using Template = cbase::Template;
  using Scheme = cbase::Scheme;

  // used to initalize a blank config file
  int make_default_config(fs::path cfg_path) {
    if (fs::exists(cfg_path)) return EXIT_FAILURE;

    std::ofstream conf;
    conf.open(cfg_path, std::ios::trunc);
    conf << "## Put each config file to be injected here, seperated by newline\n";
    conf.close();

    return EXIT_SUCCESS;
  }


  // Finds the first valid config file
  fs::path get_cbase_config() {
    // No other paths now, just return the static configuration path (with validation)
    fs::path cfg_path = CONFIG_DIR / "files.conf";
    if (!fs::exists(cfg_path)) make_default_config(cfg_path);
    return cfg_path;
  }

  // inject by reading from yaml config file in ~/.cbase/config.yaml
  // Each config file to be injected into has two "indicator" lines (can be part of a comment)
  // "%%cbase16_template_start: <template>::<subtemplate> %%"
  // "%%cbase16_template_end%%"
  std::vector<fs::path> read_cbase_config() {
    std::vector<fs::path> inj_files;

    std::ifstream conf(get_cbase_config());
    std::string line;
    int line_num = 1;
    while(std::getline(conf, line)) {
      // Check if line is a comment
      if (line.rfind("##", 0) == 0) continue;
      // if it's not, attempt to make it into a path
      fs::path fp = fs::path(line);
      if (fp.empty() || !fs::exists(fp)) throw std::runtime_error("Invalid filepath on line " + std::to_string(line_num));
      line_num++;
      inj_files.push_back(fp);
    }
    return inj_files;
  }  
 


  std::string _get_template(std::string line) {
    if (line.find("::") == std::string::npos) throw std::runtime_error("Invalid inject format, "+line); 
    size_t end = line.find("::") - 1;
    size_t start = 0;
    for (size_t i = end; i > 0; i--) 
      if (line[i] == ' ') { 
        start = i+1;
        break;
      }
    if (start == 0) throw std::runtime_error("Invalid inject format, "+line);
    return line.substr(start, end-start+1);
  }
  std::string _get_subtemplate(std::string line) {
    if (line.find("::") == std::string::npos) throw std::runtime_error("Invalid inject format, "+line); 
    size_t start = line.find("::") + 2;
    size_t end = line.length()-1;
    for (size_t i = start; i < line.length(); i++) 
      if (line[i] == ' ') { 
        end = i-1;
        break;
      }
    if (end == line.length()-1) throw std::runtime_error("Invalid inject format, "+line);
    return line.substr(start, end-start+1);
  }

  void make_backup(const fs::path& fp) {
    fs::path backup_dir = CONFIG_DIR / "backups";
    if (!fs::is_directory(backup_dir) || !fs::exists(backup_dir)) fs::create_directory(backup_dir);
    fs::path backup_fp = backup_dir / (fp.filename().string() + ".bak");
    std::ofstream os(backup_fp, std::ios::trunc);
    std::ifstream is(fp);
    os << is.rdbuf();
    os.close();
    is.close();
  }

  void inject_file(const std::shared_ptr<Scheme>& scheme, const fs::path& fp) {
    std::ifstream fs;
    std::stringstream buffer;
    fs.open(fp);
    std::string line;
    bool injecting = false;
    while (getline(fs, line)) {
      if (line.find("%%cbase16_template_start:") != std::string::npos) {
        buffer << line << '\n';
        std::shared_ptr<Template> tem = Template::findTemplate(_get_template(line));
        if (tem == nullptr) throw invalid_template(_get_template(line));
        const std::shared_ptr<Subtemplate> sub_tem = tem->getSubtemplate(_get_subtemplate(line));
        if (sub_tem == nullptr) throw invalid_template(_get_subtemplate(line), "Invalid subtemplate");
        buffer << build_template(scheme, sub_tem);
        injecting = true;
      } 
      else if (line.find("%%cbase16_template_end") != std::string::npos) injecting = false;
      if (injecting) continue;
      buffer << line << '\n';
    }
    fs.close();
    make_backup(fp);
    std::ofstream ofs;
    ofs.open(fp, std::ios::trunc);
    ofs << buffer.str();
    ofs.close();
  }

  int inject_action(const std::string& scheme, const fs::path& conf_path=CONFIG_DIR) {
    try {
      std::shared_ptr<Scheme> sch = Scheme::findScheme(scheme);
      if (sch == nullptr) throw invalid_scheme(scheme);

      std::vector<fs::path> inj_files = read_cbase_config(); 
      for (fs::path fp : inj_files) {
        inject_file(sch, fp);
      }
    } catch (invalid_scheme e) {
      std::cerr << e.what();
      return INVALID_SCHEME;
    }
    return EXIT_FAILURE;
  }

}

#endif
