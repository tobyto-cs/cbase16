#include <actions/list.hpp>

namespace cbase
{
  enum status {
    NOT_INSTALLED,
    INSTALLED,
  };

  string status_string(const status& s)
  {
    switch (s) {
      case NOT_INSTALLED:
        return "Not Installed";
      case INSTALLED:
        return "Installed";
      default:
        return "INVALID";
    }
  }
  // Use binary flags for statuses
  //   - Installed
  //   - External
  //   - Needs Updates

  int list_schemes()
  {
    // Create dictionary of scheme name, flag
    return 0;
  }

}
