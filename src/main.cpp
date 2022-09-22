#include <iostream>
#include "helpers/RootDir.h"
#include "scheme.h"
#include "template.h"
#include "macros.h"

int main() {
  std::vector<Scheme> schemes = Scheme::Builder();
  /* std::vector<Template> templates = Template::Builder(); */

  /* for (auto scheme : schemes) { */
  /*   /1* std::cout << scheme << '\n'; *1/ */
  /*   std::cout << scheme.getTag("scheme") << '\n'; */
  /* } */
  /* for (auto tmplate : templates) { */
  /*   /1* std::cout << tmplate << '\n'; *1/ */
  /*   try { */
  /*     std::cout << tmplate.getSubtemplate("default") << '\n'; */
  /*   } catch (std::invalid_argument e) { */
  /*     std::cout << e.what() << '\n'; */
  /*   } */
  /* } */

  /* Template i3 = Template::findTemplate("i3"); */
  /* std::cout << i3 << '\n'; */

  Scheme twilight = Scheme::findScheme("twilight");
  Template i3 = Template::findTemplate("i3");
  std::cout << twilight << '\n';
  std::cout << i3 << '\n';
  /* std::cout << twilight.getTag("base00") << '\n'; */

  return 0;
}
