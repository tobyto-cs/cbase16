#include "template.h"

using namespace cbase;
int main() {
  std::vector<Template::ptr> tmplates = Template::Builder();
  for (Template::ptr& tmplate : tmplates) {
    std::cout << *tmplate << '\n';
  }
  return 0;
}
