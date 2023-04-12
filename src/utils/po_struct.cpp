#include <utils/po_struct.hpp>

// REQUIRED by boost::program_options to parse std::vector for multitoken inputs
namespace std {
 std::ostream& operator<<(std::ostream &os, const boost::optional<std::vector<std::string>> &vec) {
  for (auto item : *vec) {
    os << item << " ";
  }
  return os;
}
}

namespace cbase {


po_struct::po_struct(int& argc, char* argv[], const std::string& exe_name)
  : desc(exe_name + " Options")
{
  build_option_desc(desc);
  build_variable_map(argc, argv);
}

void po_struct::build_option_desc(po::options_description& desc) {
  desc.add_options()
    ("help,h", po::value(&help)->implicit_value(std::string()), "Display is this message or detailed help on a command")
    ("list,l", po::value(&list)->implicit_value(false), "Lists installed schemes/templates")
    ("template,t", po::value(&tmplate)->implicit_value(std::string()), "Template to use for build action\n"
     "  No input: \tList out all installed templates\n"
     "  [path]: \tPath to the template\n"
     "  [name]: \tName of installed template"
     )
     ("scheme,s", po::value(&scheme)->implicit_value(std::string()), "Scheme to use for build action\n"
      "  No input: \tList out all installed schemes\n"
      "  [path]: \tPath to scheme\n"
      "  [name]: \tName of installed scheme"
      )
     ("output,o", po::value(&output), "Output file")
     ("update,u", po::value(&update)->implicit_value(std::string()), "Install and Update schemes and templates\n"
      "  No input: \tUpdate/install all offical schemes and templates\n"
      "  [path]: \tPath to a yaml file of name:git_url pairs"
       )
      ("inject,i", po::value(&inject)->multitoken()->implicit_value(std::vector<std::string>()), "Inject scheme to file(s)\n"
       "  No input: \tInject into all configured files\n"
       "  [paths]: \tList of paths to files seperated by spaces"
       )
       ("debug,d", po::value(&debug)->implicit_value(true), "Debug output");
}

bool po_struct::_help_verify() const {
  int count = 0;
  if (tmplate) count++;
  if (scheme) count++;
  if (output) count++;
  if (inject) count++;
  if (debug) count++;
  if (update) count++;
  return count <= 1 && count >= 0;
}

void po_struct::build_variable_map(int& argc, char* argv[]) {
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).run(), var_map);
    po::notify(var_map);

    // -o and -i are mutually exclusive
    if (output && inject) {
      invalid_option("Inject does not use output flag");
    }

    // help takes 0 or 1 args (not including debug)
    if (help && !_help_verify()) {
      invalid_option("Help can only display one entry at a time");
    }

    // inject only takes scheme, not template and output
    if (inject && (!scheme || tmplate || output || list || update)) {
      invalid_option("Inject requires ONLY scheme input");
    }

    // scheme and output are only valid with template specified
    if ((!inject && !help) && (!tmplate && scheme && output)) {
      invalid_option("Template must be specified");
    }

    // list is only valid with scheme OR tmplate
    if (list && (scheme && tmplate)) {
      invalid_option("List must specify --scheme OR --template");
    }

    if ((update && (tmplate && scheme)) || (output || inject)) {
      invalid_option("Update only optionally requires scheme or template");
    }

    if (debug) {
      fmt::print("Help is {}\n", help ? "ACTIVE" : "UNINITALIZED");
      fmt::print("Template is {}\n", tmplate ? *tmplate : "UNINITALIZED");
      fmt::print("Scheme is {}\n", scheme ? *scheme : "UNINITALIZED");
      fmt::print("Output is {}\n", output ? *output : "UNINITALIZED");
      fmt::print("Inject is {}\n", inject ? *output : "UNINITALIZED");
      fmt::print("Update is {}\n", update ? "ACTIVE" : "UNINITALIZED");
    }

  } catch (po::unknown_option& e) {
    invalid_option(e.what());
  } catch (po::invalid_syntax& e) {
    invalid_option(e.what());
  }
}

void po_struct::invalid_option(const std::string& msg) const {
  std::cerr << "Invalid usage: \n";
  if (!msg.empty()) std::cerr << msg << '\n';
  std::cerr << desc << '\n';
  exit(EXIT_FAILURE);
}

}
