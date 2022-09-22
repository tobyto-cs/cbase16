#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>
#include "helpers/strformat.hpp"

class CustomException : public std::exception {
  private:
    const char* message;

  public: 
    CustomException(char* msg) : message(msg) {}

    const char* what() {
      return message;
    }
};

class NonExistantScheme : public std::exception {
  private:
    std::filesystem::path path;
    const char* scheme;

  public:
    NonExistantScheme(std::filesystem::path fp, const char* scheme) : path(fp), scheme(scheme) {}

    char* what() {
      std::string msgstr = std::string("Scheme " + std::string(scheme) + " does not exist on path " + path.string()); 
      char* msg = new char[msgstr.length() + 1];
      std::strcpy(msg, msgstr.c_str());
      return msg;
    }
};

class NonExistantTemplate : public std::exception {
  private:
    std::filesystem::path path;
    const char* tmplate;

  public:
    NonExistantTemplate(std::filesystem::path fp, const char* tmplate) : path(fp), tmplate(tmplate) {}

    char* what() {
      std::string msgstr = std::string("Template " + std::string(tmplate) + " does not exist on path " + path.string()); 
      char* msg = new char[msgstr.length() + 1];
      std::strcpy(msg, msgstr.c_str());
      return msg;
    }
};
