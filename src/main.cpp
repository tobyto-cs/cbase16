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

  /* std::vector<Template> templates = Template::Builder(); */
  /* for (auto tmplate : templates) { */
  /*   /1* std::cout << tmplate << '\n'; *1/ */
  /*   try { */
  /*     std::cout << tmplate.getSubtemplate("default") << '\n'; */
  /*   } catch (std::invalid_argument e) { */
  /*     std::cout << e.what() << '\n'; */
  /*   } */
  /* } */

  std::vector<std::string> schemeList = Scheme::StringList();
  for (std::string s : schemeList) std::cout << s << '\n';
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
