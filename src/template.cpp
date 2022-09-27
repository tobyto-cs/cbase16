#include "template.h"

namespace fs = std::filesystem;

using cbase::Template;
std::vector<Template> Template::Builder(std::string fp) {
  std::vector<Template> templates;

  fs::path templatePath;
  if (!fp.empty()) templatePath = fs::path(fp);
  else templatePath = fs::path(CONFIG_DIR) / "templates";

  /* std::cout << "Template Builder Debug Start\n"; */
  /* std::cout << "TemplatePath = " << templatePath << '\n'; */
  assert(fs::exists(templatePath));

  // TODO: Find all .yaml files and try to initalize scheme
  for (const auto& file : fs::directory_iterator(templatePath)) {
    /* std::cout << file << '\n'; */
    try {
      templates.push_back(Template(file.path()));
    } catch (invalid_template e) {
      std::cout << e.what() << '\n';
    }
  }
  return templates;
}  

Template Template::findTemplate(std::string templateName) {
  fs::path templateDir = fs::path(CONFIG_DIR) / "templates";
  assert(fs::exists(templateDir));
  for (const auto& temps : fs::directory_iterator(templateDir)) {
    if (temps.path().filename() == templateName) return Template(temps.path());
  }
  throw invalid_template(templateDir/templateName, templateName);
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
    subtemplates.emplace_back(Subtemplate(n, stfp));
  }
}

const Template::Subtemplate Template::getSubtemplate(std::string name) const {
  auto it = std::find_if(subtemplates.begin(), subtemplates.end(), [&name](Template::Subtemplate subtemp){ return subtemp.isEqual(name); }); 
  if (it == subtemplates.end()) throw invalid_template("Subtemplate is not found...");
  return *it;
}

Template::Subtemplate::Subtemplate(ryml::NodeRef subtemplateRoot, std::filesystem::path fp) {
  c4::from_chars(subtemplateRoot.key(), &name);
  if (!fs::exists(fp)) throw invalid_template(fp, name);
  
  std::filesystem::path mustache = fp/"templates"/(name + ".mustache");
  c4::from_chars(subtemplateRoot.find_child("extension").val(), &extension);
  c4::from_chars(subtemplateRoot.find_child("output").val(), &outputPath);
  outputPath = fp / outputPath;
}

const std::string Template::Subtemplate::getName() const { return name; }
const std::string Template::Subtemplate::getExtension() const { return extension; }
const std::string Template::Subtemplate::getOutputPath() const { return outputPath; }
const std::string Template::Subtemplate::getMustache() const { return mustache; }
