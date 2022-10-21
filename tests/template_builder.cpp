#include "template.h"

using namespace cbase;
int main() {
  std::vector<std::shared_ptr<Template>> tmplates = Template::Builder();
  for (auto& tmplate : tmplates) {
    std::cout << *tmplate << '\n';
  }
  return 0;
}
