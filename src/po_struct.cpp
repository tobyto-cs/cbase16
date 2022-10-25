#include "po_struct.hpp"

namespace cbase {
  po_struct::po_struct(int& argc, char* argv[]) {
    // Build the options and their descriptions
    this->desc.add(build_option_desc());
    // Build the positional options description
    this->pos_desc.add("action", 1);
    // Construct the variable map
    build_variable_map(argc, argv);
  }

  const void po_struct::invalid_option(const std::string& msg) const {
    if (!msg.empty()) std::cout << msg << '\n';
    std::cout << "Invalid usage: \n";
    std::cout << desc << '\n';
    exit(EXIT_FAILURE);
  }

  std::string po_struct::get_type() {
    std::string type = get_option<std::string>("type");
    std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){ return std::tolower(c); });
    if (type != "scheme" && type != "template") invalid_option("Invalid type");
    return type;
  }

  po::options_description po_struct::build_option_desc() {
    po::options_description general("General Options");
    general.add_options()
      ("help,h", po::value<std::string>()->implicit_value(""), "Display this message or details on a command")
      ("action", po::value<std::string>()->default_value(""), "[options]")
      ("debug,d", po::value<bool>()->default_value(false), "Print out debug stuffs");

    po::options_description list("List Action Options");
    list.add_options()
      ("type,T", po::value<std::string>(), "What to list\nvalid options = [\"scheme\", \"template\"]");

    po::options_description build("Build Action Options");
    build.add_options()
      ("template,t", po::value<std::string>(), "Template to use for build action\n"
       "  [path]: \tPath to template\n"
       "  [name]: \tName of installed template"
      )
      ("subtemplate,S", po::value<std::string>()->default_value("default"), "Subtemplate of Template to use, defaults to \"default\""
       "  [name]: \tValid name of subtemplate, found inside template config.yaml"
      )
      ("output,o", po::value<std::string>()->default_value(STDOUT), "Optional built template output path\n"
       "  no value: \tOutputs on stdout\n"
       "  default: \tOutputs on the default template path in the config.yaml\n"
       "  [path]: \tSets output path to [path]"
      )
      ("scheme,s", po::value<std::string>()->default_value(STDIN), "Scheme to use for build action\n"
       "  no value: \tInput from stdin\n"
       "  [path]: \tPath to scheme directory\n"
       "  [name]: \tName of installed scheme"
      );

    po::options_description inject("Inject Action Options");
    inject.add_options()
      ("config,c", po::value<std::string>()->default_value(""), "Optional path to injection config file\n"
       "  no value: \tuse deafult config file (from default paths)\n"
       "  [path]: \tpath to cbase16.yaml configuration file"
      );
    po::options_description desc("cbase16 - base16 builder and manager");
    desc.add(general).add(list).add(build).add(inject);
    return desc;
  }

  void po_struct::build_variable_map(int& argc, char* argv[]) {
    // Try to read cli input, if invalid output the usage description
    try {
      po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), var_map);
      po::notify(var_map);
    } catch (po::unknown_option e) {
      invalid_option();
    }
  }
}
