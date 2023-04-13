#ifndef CBASE_INJECT_ACTION
#define CBASE_INJECT_ACTION

#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <ryml.hpp>
#include <ryml_std.hpp>

#include <scheme/scheme.hpp>
#include <utils/po_struct.hpp>
#include <actions/build.hpp>

namespace fs = std::filesystem;

using std::string, std::ofstream, std::vector;

namespace cbase
{
  // Inject script looks for these identifiers in a string
  const inline string START_IDENTIFIER = "cbase16::start";
  const inline string END_IDENTIFIER = "cbase16::end";

  int inject_action(const po_struct& args);
}

#endif
