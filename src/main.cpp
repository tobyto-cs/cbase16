#include <iostream>
#include "helpers/RootDir.h"
#include "helpers/macros.h"
#include "scheme.h"
#include "template.h"
#include <boost/program_options.hpp>

using namespace cbase;
namespace po = boost::program_options;

void invalid_cli(po::options_description desc);

int main(int argc, char* argv[]) {
  po::options_description desc("cbase16 cli tool usage");
  desc.add_options()
    ("help,h", "Display this help message")
    ("version,v", "Display the version number")
    ("action", po::value<std::string>(), "Determines what action to do" 
        "Values: \n"
        "\tbuild <template> <scheme>: \n"
        "\tinject:")
    ("args", po::value<std::vector<std::string>>(), "Arguments for action")
  po::positional_options_description pd;
  pd.add("action", 1).add("args", -1);
 
  po::variables_map vm;
  // Try to read cli input, if invalid output the usage description
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
    po::notify(vm);
  } catch (po::unknown_option e) {
    invalid_cli(desc);
  }

  if (vm.count("help")) {
    std::cout << desc << '\n';
    return 0;
  } 

  if (vm.count("action")) {
    std::vector<std::string> args = vm["args"].as<std::vector<std::string>>();
    std::string action = vm["action"].as<std::string>();

    if (action == "list") {


    } else if (action == "build") {

    } else if (action == "inject") {

    } else {
      invalid_cli(desc);
    }
  }

  return 0;
}

void invalid_cli(po::options_description desc) {
  std::cout << "Invalid usage...\n"; 
  std::cout << desc << '\n';
  exit(1);
}
