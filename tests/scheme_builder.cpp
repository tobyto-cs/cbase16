#include "scheme.h"

using namespace cbase;
int main() {
  std::vector<Scheme::ptr> schemes = Scheme::Builder();
  for (Scheme::ptr& scheme : schemes) {
    std::cout << *scheme << '\n';
  }
  return 0;
}
