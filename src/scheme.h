#ifndef SCHEME
#define SCHEME

#include <set>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <ryml.hpp>
#include <ryml_std.hpp>

class Scheme {
  private:
    static const std::set<std::string> validTags;
    static bool isValidTag(const std::string tag) {
      if (validTags.find(tag) != validTags.end()) return true;
      else return false;
    }
    ryml::Tree stree;
    std::map<std::string, std::string> tags;
  
  public:
    /*
     * @param fn Full path to the .yaml scheme 
     */
    Scheme(std::string fn);
    /*
     * @param fp If fp is supplied (not NULL) then check dir for valid schemes
     */
    static std::vector<Scheme> Builder(std::string fp="");
    friend std::ostream& operator<<(std::ostream& o, const Scheme& scheme) {
      for (auto tag : scheme.tags) {
        o << tag.first << '=' << tag.second << '\n';
      }
      return o;
    }
    const std::string getTag(const std::string& tag);
    const static Scheme findScheme(std::string schemeName);
};

#endif
