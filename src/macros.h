#ifndef MACROS
#define MACROS

#include <string>
#include <filesystem>
#include <assert.h>

// TODO: Include alternative unix home enviroments
const std::filesystem::path CONFIG_DIR = std::filesystem::path(std::string("/.cbase16/").insert(0, getenv("HOME")));

#endif
