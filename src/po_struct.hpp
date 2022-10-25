#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include "helpers/macros.h"

namespace po = boost::program_options;

namespace cbase {
  struct po_struct {
    po::options_description desc;
    po::variables_map var_map;
    po::positional_options_description pos_desc;
 
    po_struct(po::options_description desc, po::variables_map var_map) : desc(desc), var_map(var_map) {}
    po_struct(int& argc, char* argv[]);

    po::options_description build_option_desc();
    void build_variable_map(int& argc, char* argv[]);

    const void invalid_option(const std::string& msg=std::string()) const;
    const bool has_option(const std::string& option) { return var_map.count(option); }
    template <typename T> T get_option(const std::string& option) { return has_option(option) ? var_map[option].as<T>() : T(); }
    std::string get_help_arg() { return get_option<std::string>("help"); }
    std::string get_action() { return get_option<std::string>("action"); }
    bool is_debug() { return get_option<bool>("debug"); }
    std::string get_type();
    std::string get_template() { return get_option<std::string>("template"); }
    std::string get_subtemplate() { return get_option<std::string>("subtemplate"); }
    std::string get_scheme() { return get_option<std::string>("scheme"); }
    std::string get_output() { return get_option<std::string>("output"); }
    std::string get_config() { return get_option<std::string>("config"); }
  };
}
