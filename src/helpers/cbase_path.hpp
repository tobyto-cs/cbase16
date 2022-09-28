#ifndef CBASE_PATH
#define CBASE_PATH

#include <filesystem>
#include <functional>
#include <assert.h>
#include <iostream>  

#include "macros.h"

namespace fs = std::filesystem;
namespace cbase {
  /*
   * @param fp path of dir to recursivly iterate based on base16 standards
   * @param dir_func function to run with the input being the current file's path
   */
  void schemedir_parser(const fs::path fp, const std::function<void(fs::path)> dir_func);
  /*
   * @param fp path of dir to recursivly iterate based on base16 standards
   * @param dir_func function to run with the input being the current file's path
   */
  void templatedir_parser(const fs::path fp, const std::function<void(fs::path)> dir_func);
  /*
   * @param fp path of dir to check
   *           if not given default to config dir
   *
   * @return valid path to check for schemes
   */
  fs::path fp_checker(const std::string& fp, const std::string& type);
  fs::path schemefp_checker(const std::string& fp);
  fs::path templatefp_checker(const std::string& fp);

}

#endif
