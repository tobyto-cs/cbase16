#ifndef CBASE_MACROS
#define CBASE_MACROS

#include <string>
#include <filesystem>

using std::string;
namespace fs = std::filesystem;

const fs::path CBASE_CONFIG_DIR = fs::path(string("/.config/cbase16/").insert(0, getenv("HOME")));
const fs::path CBASE_FILES_LIST = CBASE_CONFIG_DIR / "files.conf";
const fs::path CBASE_TEMPLATES_DIR = CBASE_CONFIG_DIR / "templates";
const fs::path CBASE_SCHEMES_DIR = CBASE_CONFIG_DIR / "schemes";

const string CBASE_TEMPLATES_URL = "https://raw.githubusercontent.com/chriskempson/base16-templates-source/master/list.yaml";
const fs::path CBASE_TEMPLATES_LIST = CBASE_TEMPLATES_DIR / "list.yaml";
const string CBASE_SCHEMES_URL = "https://raw.githubusercontent.com/chriskempson/base16-schemes-source/main/list.yaml";
const fs::path CBASE_SCHEMES_LIST = CBASE_SCHEMES_DIR / "list.yaml";

#define INVALID_USAGE 4
#define INVALID_SCHEME 5
#define INVALID_TEMPLATE 6
#define INVALID_SUBTEMPLATE 7

#endif
