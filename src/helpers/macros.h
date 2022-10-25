#ifndef CBASE_MACROS
#define CBASE_MACROS

#include <string>
#include <filesystem>

const std::filesystem::path CONFIG_DIR = std::filesystem::path(std::string("/.cbase16/").insert(0, getenv("HOME")));
const std::filesystem::path FILES_LIST = CONFIG_DIR / "files.conf";
const std::filesystem::path TEMPLATES_DIR = CONFIG_DIR / "templates";
const std::filesystem::path SCHEMES_DIR = CONFIG_DIR / "schemes";
#define STDIN std::string("stdin")
#define STDOUT std::string("stdout")
#define INVALID_USAGE 4
#define INVALID_SCHEME 5
#define INVALID_TEMPLATE 6
#define INVALID_SUBTEMPLATE 7

#endif
