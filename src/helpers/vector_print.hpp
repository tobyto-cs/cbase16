#ifndef CBASE_VECTOR_PRINT
#define CBASE_VECTOR_PRINT

#define COL_PADDING 4
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <cmath>


namespace cbase {
  std::ostream& fancy_print(std::ostream& o, std::vector<std::string> container) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    unsigned short term_row_sz = w.ws_row;
    unsigned short term_col_sz = w.ws_col;

    //find length of largest string in container
    auto it = std::max_element(container.begin(), container.end(), [](const auto& a, const auto& b){
        return a.size() < b.size();
        });
    size_t col_sz = it->size() + COL_PADDING;
    size_t num_cols = std::floor(term_col_sz / col_sz); 

    int col_pos = 1;
    size_t str_sz;
    for (auto& str : container) {
      if (col_pos > num_cols) { 
        o << '\n';
        col_pos = 1;
        continue;
      }

      if (col_pos == num_cols) o << str;
      else {
        str_sz = str.size();
        o << str << std::string(col_sz-str_sz, ' ');
      }
      col_pos++;
    }
    return o << '\n';
  }
}

#endif
