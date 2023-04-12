#include <iostream>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <utils/macros.hpp>

#include <actions/build.hpp>
#include <actions/list.hpp>
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


  if (args.help)
  {
    cout << args.desc << '\n';
    exit(EXIT_SUCCESS);
  }

  else if (args.list)
  {
    if (args.scheme)
    {
      return cbase::list_schemes(args.debug.is_initialized());
    }
    else if (args.tmplate)
    {
      return cbase::list_templates(args.debug.is_initialized());
    }
    else if (args.update)
    {
      exit(cbase::check_updates(args.debug.is_initialized()));
    }
  }

  else if (args.update) {
    if (*args.debug) cout << fmt::format("Going to do update\n");
    exit(cbase::do_updates(*args.debug));
  }

  else if (args.tmplate) {
    if (args.debug) fmt::print("Build_Action\n");

    // Check for scheme (STDIN or by tag)
    bool piped = !isatty(STDIN_FILENO);
    if (!args.scheme && !piped) args.invalid_option("Please specify a scheme to use, from Standard Input or by file");
    if (piped && args.scheme) args.invalid_option("Scheme specified twice, use either Standard Input or --scheme");

    cbase::build_action(args);
  }

  else if (args.inject) {
    // Inject action needs:
    //   - Scheme
  }

  return 0;
}
