#ifndef CBASE_PO_STRUCT
#define CBASE_PO_STRUCT

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <fmt/core.h>

namespace po = boost::program_options;
template<class T> using optional = boost::optional<T>;

namespace cbase {
  struct po_struct {
    po::options_description desc;
    po::variables_map var_map;

    optional<std::string> help;
    optional<std::string> tmplate;
    optional<std::string> scheme;
    optional<std::string> output;
    optional<std::string> update;
    optional<std::vector<std::string>> inject;
    optional<bool> debug;
    optional<bool> list;

    po_struct(po::options_description desc, po::variables_map var_map) : desc(desc), var_map(var_map) {}
    po_struct(int& argc, char* argv[], const std::string& exe_name);

    void build_option_desc(po::options_description& desc);
    void build_variable_map(int& argc, char* argv[]);

    void invalid_option(const std::string& msg=std::string()) const;
    bool has_option(const std::string& option) { return var_map.count(option); }
    template <typename T> T get_option(const std::string& option) { return has_option(option) ? var_map[option].as<T>() : T(); }

    private:
      bool _help_verify() const;
  };
}

#endif
