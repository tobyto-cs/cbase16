#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <assert.h>
#include <system_error>
#include "helpers/RootDir.h"
#include "macros.h"
#include "scheme.h"


namespace fs = std::filesystem;

const std::set<std::string> Scheme::validTags = {"scheme", "author", "base00", "base01", "base02", 
      "base03", "base04", "base04", "base05", "base06", "base07", "base08", "base09", "base0A", "base0B", "base0C", "base0D", "base0E", "base0F"};

std::vector<Scheme> Scheme::Builder(std::string fp) {
  std::vector<Scheme> schemes;

  fs::path schemesPath;
  if (!fp.empty()) schemesPath = fs::path(fp);
  else schemesPath = fs::path(CONFIG_DIR) / "schemes";

  /* std::cout << "Scheme Builder Debug Start\n"; */
  /* std::cout << "SchemePath = " << schemesPath << '\n'; */
  assert(fs::exists(schemesPath));

  // TODO: Find all .yaml files and try to initalize scheme
  for (const auto & templatedir : fs::directory_iterator(schemesPath)) {
    for (const auto & file : fs::directory_iterator(templatedir.path())) {
      if (file.path().extension() == ".yaml") {
        try {
          schemes.push_back(Scheme(file.path()));
        } catch (std::invalid_argument e) {
          std::cout << e.what() << std::endl;
          continue;
        }
      }
    }
  }

  return schemes;
}

const Scheme Scheme::findScheme(std::string schemeName) {
  fs::path schemesPath = fs::path(CONFIG_DIR)/"schemes";

  std::string schemeYaml = schemeName + ".yaml";
  if (fs::exists(schemesPath/schemeName) && fs::exists(schemesPath/schemeName/schemeYaml)) {
    return Scheme(schemesPath / schemeName / schemeYaml);
  } else {
    for (const auto& schemeDir : fs::directory_iterator(schemesPath)) {
      for (const auto& file : fs::directory_iterator(schemeDir.path())) {
        if (file.path().extension() == ".yaml") return Scheme(file.path()); 
      }
    }
  }
  throw std::invalid_argument("Scheme of name "+schemeName+" Does not exist");
}

Scheme::Scheme(std::string fn) {
  fs::path schemePath = fn;

  // Parse the .yaml file
  std::ifstream file(schemePath);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  stree = ryml::parse_in_arena(ryml::to_csubstr(content));

  // Grab and validate keys
  std::vector<ryml::substr> keys;
  std::string keyBuf, valBuf;
  int numValidTags = 0;
  auto it = validTags.begin();
  for (ryml::NodeRef n : stree.rootref().children()) {
    c4::from_chars(n.key(), &keyBuf);
    if (!isValidTag(keyBuf)) std::cout << fn << " contains invalid key: " << keyBuf <<'\n';  
    if (!n.has_val()) throw std::invalid_argument(fn + " is an invalid scheme, tag " + keyBuf + " has no value");  
    numValidTags++;
    c4::from_chars(n.val(), &valBuf);
    tags.insert(std::pair<std::string, std::string>(keyBuf, valBuf));
  }
  if (numValidTags != validTags.size()) {
    throw std::invalid_argument(fn + " does not contain all required tags");
  }
}

const std::string Scheme::getTag(const std::string& tag) {
  auto it = tags.find(tag);
  if (it != tags.end()) return it->second;  
  else return "";
}







