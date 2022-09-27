#ifndef CBASE_EXCEPTIONS
#define CBASE_EXCEPTIONS

#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>
#include "helpers/strformat.hpp"

namespace fs = std::filesystem;
namespace cbase {
  // Abstract baseclass
  class invalid_obj : public std::exception {
    protected:
      const std::string type;

    private:
      fs::path path;
      const char* name;
      const char* msg;

      const std::string get_msgstr() {
        std::string msgstr = type + " " + name + "is invalid:";
        if (!fs::is_empty(path)) msgstr += " does not exist on path " + path.string();
        if (msg) msgstr += "\n" + std::string(msg);
        return msgstr;
      }

    public:
      invalid_obj(const std::string& type, const char* name, const fs::path& fp = fs::path(), const char* msg = "") : 
        path(fp), name(name), msg(msg), type(type) {}
      invalid_obj(const std::string& type, const std::string& name, const fs::path& fp = fs::path(), const std::string& msg = "") : 
        path(fp), name(name.c_str()), msg(msg.c_str()), type(type) {}

      char* what() {
        std::string msgstr = get_msgstr();
        char* whatmsg = new char[msgstr.length() + 1];
        std::strcpy(whatmsg, msgstr.c_str());
        return whatmsg;
      }
  };
  class invalid_scheme : public invalid_obj {
    public:
      invalid_scheme(const char* scheme, const std::filesystem::path& fp = fs::path(), const char* msg="") : 
        invalid_obj("Scheme", scheme, fp, msg) {}
      invalid_scheme(const std::string& scheme, const std::filesystem::path& fp = fs::path(), const std::string& msg="") : 
        invalid_obj("Scheme", scheme, fp, msg) {}
  };

  class invalid_template : public invalid_obj {
    public:
      invalid_template(const char* tmplate, const std::filesystem::path& fp = fs::path(), const char* msg="") : 
        invalid_obj("Template", tmplate, fp, msg) {}
      invalid_template(const std::string& tmplate, const std::filesystem::path& fp = fs::path(), const std::string& msg="") : 
        invalid_obj("Template", tmplate, fp, msg) {}
  };
}

#endif
