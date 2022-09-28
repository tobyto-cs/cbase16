#include "scheme.h"

namespace fs = std::filesystem;
using cbase::Scheme;

// ---- CONST VARIABLES ----

const std::set<std::string> Scheme::validTags = {"scheme", "author", "base00", "base01", "base02", 
  "base03", "base04", "base04", "base05", "base06", "base07", "base08", "base09", "base0A", "base0B", "base0C", "base0D", "base0E", "base0F"};

// ==== STATIC FUNCTIONS ====

std::vector<std::string> Scheme::StringList(const std::string& fp) {
  std::vector<std::string> schemes;
  schemedir_parser(schemefp_checker(fp), [&schemes](fs::path file) -> void {
    if (file.extension() == ".yaml") schemes.emplace_back(file.stem());
  });
  return schemes;
}

std::vector<fs::path> Scheme::PathList(const std::string& fp) {
  std::vector<fs::path> schemes;

  schemedir_parser(schemefp_checker(fp), [&schemes](fs::path file) -> void {
    if (file.extension() == ".yaml") schemes.emplace_back(file);
  });
  return schemes;
}

std::vector<Scheme::ptr> Scheme::Builder(const std::string& fp) {
  std::vector<std::unique_ptr<Scheme>> schemes;
  // Parse all scheme directories
  schemedir_parser(schemefp_checker(fp), [&schemes](fs::path file) -> void {
    if (file.extension() != ".yaml") return;
    assert(fs::is_regular_file(file));
    try { 
      schemes.emplace_back(std::move(ptr(new Scheme(file))));
    }
    catch (invalid_scheme e) { 
      std::cout << e.what() << '\n';
    }
  });
  return schemes;
}

// ==== CLASS METHODS ====

const Scheme::ptr Scheme::findScheme(std::string schemeName) {
  fs::path schemesPath = fs::path(CONFIG_DIR)/"schemes";

  std::string schemeYaml = schemeName + ".yaml";
  if (fs::exists(schemesPath/schemeName) && fs::exists(schemesPath/schemeName/schemeYaml)) {
    return ptr(new Scheme(schemesPath / schemeName / schemeYaml));
  } else {
    for (const auto& schemeDir : fs::directory_iterator(schemesPath)) {
      for (const auto& file : fs::directory_iterator(schemeDir.path())) {
        if (file.path().extension() == ".yaml") return ptr(new Scheme(file.path())); 
      }
    }
  }
  return NULL;
}


const std::string Scheme::getTag(const std::string& tag) {
  auto it = tags.find(tag);
  if (it != tags.end()) return it->second;  
  else return "";
}

// ==== CONSTRUCTORS ====

Scheme::Scheme(std::string fn) {
  fs::path schemePath = fn;

  parent_dir = schemePath.parent_path();

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


