#include <iostream>
#include <boost/program_options.hpp>

#include "helpers/RootDir.h"
#include "helpers/macros.h"
#include "scheme.h"
#include "template.h"
#include "actions/list.hpp"
#include "actions/build.hpp"

using namespace cbase;
namespace po = boost::program_options;

void invalid_cli(const po::options_description& desc);
int build_handler(const po::variables_map& vm, const std::vector<std::string>& args);

int main(int argc, char* argv[]) {
  po::options_description desc("cbase16 cli tool usage");
  desc.add_options()
    ("help,h", "Display this help message")
    ("version,v", "Display the version number")
    ("action", po::value<std::string>(), "Determines what action to do" 
        "\nValues: \n"
        "\t   build <template> <scheme>: \n"
        "\t   inject:")
    ("type,T", po::value<std::string>(), "Either scheme or template for list action")
    ("template,t", po::value<std::string>(), "Template for build action")
    ("args", po::value<std::vector<std::string>>(), "Arguments for action");
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
    std::string action = vm["action"].as<std::string>();
    std::vector<std::string> args;
    if (vm.count("args")) args = vm["args"].as<std::vector<std::string>>();

    if (action == "list") {
      std::string type = "scheme";
      if (vm.count("type")) type = vm["type"].as<std::string>();
      return list_action(type);
    } else if (action == "build") {
      assert(args.size() == 2);
      try {
        std::cout << "Building template " << args[0] << " with scheme " << args[1] << '\n';
        return build_action(args[0], args[1]); 
      } catch (std::runtime_error const e) {
        std::cout << "Caught an exception in build_action:\n";
        std::cout << "    " << e.what() << '\n'; 
        return 1;
      } catch (std::exception e) {
        std::cout << "Caught an exception in build_action:\n";
        std::cout << "    " << e.what() << '\n';
        return 1;
      }
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
