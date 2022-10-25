#include "template.h"

namespace fs = std::filesystem;

using cbase::Template;
std::vector<std::shared_ptr<Template>> Template::Builder(std::string fp) {
  std::vector<std::shared_ptr<Template>> templates;

  templatedir_parser(templatefp_checker(fp), [&templates](fs::path fp) -> void {
    try {
      templates.emplace_back(std::make_shared<Template>(fp));
    } catch (invalid_template e) {
      std::cout << e.what() << '\n';
    }
  }); 
  return templates;
}  

std::vector<std::string> Template::StringList(std::string fp) {
  std::vector<std::string> tmplates;
  templatedir_parser(templatefp_checker(fp), [&tmplates](fs::path fp) -> void {
    std::string tmp_name = fp.stem();
    for (const auto& file : fs::directory_iterator(fp/"templates")) {
      if (file.path().extension() == ".mustache") tmplates.emplace_back(tmp_name+"::"+file.path().stem().string());
    }
  });
  return tmplates;
}

std::shared_ptr<Template> Template::findTemplate(std::string templateName) {
  if (!fs::exists(TEMPLATES_DIR)) throw invalid_template(templateName, TEMPLATES_DIR, "Cannot find the template");
  for (const auto& temps : fs::directory_iterator(TEMPLATES_DIR)) {
    if (temps.path().filename() == templateName) return std::make_shared<Template>(temps.path());
  }
  return nullptr;
}


Template::Template(std::string fn) {
  name = fs::path(fn).filename();
  fs::path configPath = fs::path(fn) / "templates/config.yaml";
  if (!fs::exists(configPath)) throw invalid_template(fn, name, "Config yaml file not found...");
  /* std::cout<<"\nParsing "<<configPath<<'\n'; */

  std::ifstream file(configPath);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(content));

  ryml::NodeRef root = tree.rootref();
  for(ryml::NodeRef n : root.children()) {
    /* std::cout<<n<<'\n'; */
    std::string nodeName;
    c4::from_chars(n.key(), &nodeName);

    // TODO: validate this path it finds
    fs::path stfp = fs::path(fn);
    /* std::cout << "Added subtemplate with path: " << mustache.c_str() << '\n'; */
    subtemplates.emplace_back(std::make_shared<Subtemplate>(Subtemplate(n, stfp, this->name)));
  }
}

const std::shared_ptr<Template::Subtemplate> Template::getSubtemplate(std::string name) const {
  auto it = std::find_if(subtemplates.begin(), subtemplates.end(), [&name](std::shared_ptr<Template::Subtemplate> subtemp){ return subtemp->isEqual(name); }); 
  if (it == subtemplates.end()) return nullptr;
  return *it;
}

const std::vector<std::string> Template::listSubtemplate() const {
  std::vector<std::string> list;
  for (auto sub_tem : subtemplates) list.emplace_back(sub_tem->getName());
  return list;
}

Template::Subtemplate::Subtemplate(ryml::NodeRef subtemplateRoot, std::filesystem::path fp, const std::string& tempName) {
  templateName = tempName;
  c4::from_chars(subtemplateRoot.key(), &name);
  if (!fs::exists(fp)) throw invalid_template(fp, name);
  
  std::string mustache_file = std::string(name + ".mustache");
  mustache = fp/"templates"/mustache_file;
  /* std::cout << "mustacepath = " << mustache << '\n'; */
  c4::from_chars(subtemplateRoot.find_child("extension").val(), &extension);
  c4::from_chars(subtemplateRoot.find_child("output").val(), &outputPath);
  outputPath = fp / outputPath;
}

const std::string Template::Subtemplate::getName() const { return name; }
const std::string Template::Subtemplate::getExtension() const { return extension; }
const std::string Template::Subtemplate::getOutputPath() const { return outputPath; }
const std::string Template::Subtemplate::getMustache() const { return mustache; }
