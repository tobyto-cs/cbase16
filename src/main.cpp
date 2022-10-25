#include <iostream>
#include <boost/program_options.hpp>

#include "helpers/RootDir.h"
#include "helpers/macros.h"
#include "scheme.h"
#include "template.h"
#include "actions/list.hpp"
#include "actions/build.hpp"
#include "actions/inject.hpp"
#include "po_struct.hpp"

using namespace cbase;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  // Parses the command line inputs
  po_struct args = po_struct(argc, argv);
  ushort result;

  if (args.has_option("help")) {
    std::cout << "Displaying Help\n";
    std::cout << args.desc << '\n';
    return 0;
  } 
  if (args.has_option("action")) {
    std::string action = args.get_action();

    if (action == "list") { return list_action(args.get_type()); } 
    if (action == "build") {
      result = build_action(args.get_scheme(), args.get_template(), args.get_subtemplate(), args.get_output()); 
      if (result == INVALID_USAGE) args.invalid_option("Invalid usage, check stderr");
      return result;
    }
    if (action == "inject") {
      result = inject_action(args.get_scheme());
      return 0;
    }
    // If action is not caught beforehand
    args.invalid_option("Invalid action, " + action);
  }

  return 0;
}
