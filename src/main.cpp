#include <iostream>
#include "helpers/RootDir.h"
#include "helpers/macros.h"
#include "scheme.h"
#include "template.h"

using namespace cbase;
int main() {

  /* std::vector<Scheme::ptr> schemes = Scheme::Builder(); */
  /* for (Scheme::ptr& scheme : schemes) { */
  /*   std::cout << *scheme << '\n'; */
  /* } */

  std::vector<Template::ptr> templates = Template::Builder();
  for (Template::ptr& tmplate : templates) {
    Template::sub_ptr default_subtemp = tmplate->getSubtemplate("default");
    if (default_subtemp == NULL) std::cout << tmplate->getName() << " does not have a default subtemplate" << '\n';
    std::cout << *tmplate << '\n';
  }

  /* std::vector<std::string> schemeList = Scheme::StringList(); */
  /* for (std::string s : schemeList) std::cout << s << '\n'; */
  /* std::vector<std::filesystem::path> schemePathList = Scheme::PathList(); */
  /* for (auto fp : schemePathList) std::cout << fp << '\n'; */

  /* Template i3 = Template::findTemplate("i3"); */
  /* std::cout << i3 << '\n'; */

  /* Scheme::ptr twilight = Scheme::findScheme("twilight"); */
  /* Template i3 = Template::findTemplate("i3"); */
  /* std::cout << *twilight << '\n'; */
  /* std::cout << i3 << '\n'; */
  /* std::cout << twilight.getTag("base00") << '\n'; */

  return 0;
}
