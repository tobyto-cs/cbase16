#include <iostream>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <utils/macros.hpp>

#include <actions/build.hpp>
#include <actions/list.hpp>
#include <actions/inject.hpp>
#include <utils/po_struct.hpp>

// Uses libgit2
#include <utils/updater.hpp>

#include <fmt/core.h>

extern "C" {
#include <git2.h>
}

namespace po = boost::program_options;
using std::cout;

int main(int argc, char* argv[])
{

  cbase::po_struct args = cbase::po_struct(argc, argv, "cbase16");

  int exit_code = 0;
  try {
local NeoTree = {

}
    if (args.help)
    {
      cout << args.desc << '\n';
      exit_code = EXIT_SUCCESS;
    }

    else if (args.list.get())
    {
      if (args.scheme)
      {
        exit_code = cbase::list_schemes(args.debug.get());
      }
      else if (args.tmplate)
      {
        exit_code = cbase::list_templates(args.debug.get());
      }
      else if (args.update)
      {
        exit_code = cbase::check_updates(args.debug.get());
      }
    }

    else if (args.update) {
      exit_code = cbase::do_updates(args.debug.get());
    }

    else if (args.tmplate) {
      if (args.debug) fmt::print("Build_Action\n");

      // Check for scheme (STDIN or by tag)
      bool piped = !isatty(STDIN_FILENO);
      if (!args.scheme && !piped) args.invalid_option("Please specify a scheme to use, from Standard Input or by file");
      if (piped && args.scheme) args.invalid_option("Scheme specified twice, use either Standard Input or --scheme");

      exit_code = cbase::build_action(args);
    }

    else if (args.inject.get()) {
      bool piped = !isatty(STDIN_FILENO);
      if (!args.scheme && !piped) args.invalid_option("Please specify a scheme to use, from Standard Input or by file");
      if (piped && args.scheme) args.invalid_option("Scheme specified twice, use either Standard Input or --scheme");

      exit_code = cbase::inject_action(args);
    }
  } catch (std::runtime_error& e) {
    fmt::print("Unhandled runtime error\n{}\n", e.what());
    exit_code = EXIT_FAILURE;
  }

  return exit_code;
}
