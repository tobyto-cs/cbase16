#ifndef CBASE_SCHEME
#define CBASE_SCHEME

#include <set>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include <assert.h>
#include <functional>
#include <unistd.h>
#include <stdio.h>

// ryml headers
#include <ryml.hpp>
#include <ryml_std.hpp>

// cbase headers
#include "helpers/RootDir.h"
#include "helpers/macros.h"
#include "helpers/cbase_path.hpp"
#include "exceptions.hpp"

namespace cbase {
  class Scheme {
    private:
      static const std::set<std::string> validTags;
      static bool isValidTag(const std::string tag) {
        if (validTags.find(tag) != validTags.end()) return true;
        else return false;
      }
      ryml::Tree stree;
      std::map<std::string, std::string> tags;
      std::filesystem::path parent_dir;
      void build_from_stream(std::istream& stream, const std::string& id="stream");

    public:
      /*
       * @param fn Full path to the .yaml scheme 
       */
      Scheme(const std::string& fn);
      Scheme(std::istream& inp_stream);
      
      /*
       * @param fp If fp is supplied (not NULL) then check dir for valid schemes
       *
       * @return a vector of all std::unique_ptr<Scheme> or all valid schemes from default path
       */
      static std::vector<std::shared_ptr<Scheme>> Builder(const std::string& fp="");

      /*
       * @param fp If fp is supplied, then check dir for valid schemes
       *
       * @return a sorted vector of a list of all strings
       */
      static std::vector<std::string> StringList(const std::string& fp="");
      static std::vector<std::filesystem::path> PathList(const std::string& fp="");

      /*
       * Attempts to find a valid scheme from the preset config paths.
       *
       * @param schemeName Name of scheme to find from config paths
       *
       * @return A std::unique_ptr<Scheme> to the new scheme
       *         NULL if scheme is not found
       */
      const static std::shared_ptr<Scheme> findScheme(std::string schemeName);

      friend std::ostream& operator<<(std::ostream& o, const Scheme& scheme) {
        for (auto tag : scheme.tags) {
          o << tag.first << '=' << tag.second << '\n';
        }
        return o << "Parent path = " << scheme.parent_dir << '\n';
      }

      /*
       * @param tag A "validTag" to get the value from
       *
       * @return string that contains "value" of tag
       *         empty string if tag is invalid
       */
      const std::string getTag(const std::string& tag);

  };
}

#endif
