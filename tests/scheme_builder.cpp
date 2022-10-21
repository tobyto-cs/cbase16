#include "scheme.h"

using namespace cbase;
int main() {
  std::vector<std::shared_ptr<Scheme>> schemes = Scheme::Builder();
  for (auto& scheme : schemes) {
    std::cout << *scheme << '\n';
  }
  return 0;
}
