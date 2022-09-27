#ifndef CBASE_TEMPLATE
#define CBASE_TEMPLATE

#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
// ryml lib headers
#include <ryml.hpp>
#include <ryml_std.hpp>

#include "scheme.h"

#include "helpers/macros.h"
#include "exceptions.hpp"
/**
 * A class that stores a templates colors
 */
namespace cbase {
  class Template {
    public: 
      class Subtemplate {
        private:
          std::string name, extension, outputPath;
          std::filesystem::path mustache;
        public: 
          /*
           * @param subtemplateRoot   YAML root node of the subtemplate
           * @param fp                Filepath to the .mustache file
           */
          Subtemplate(ryml::NodeRef subtemplateRoot, std::filesystem::path fp);


          // GETTERS
          const std::string getName() const;
          const std::string getExtension() const;
          const std::string getOutputPath() const;
          const std::string getMustache() const;


          // HELPERS
          friend std::ostream& operator<<(std::ostream& o, const Subtemplate& subtemp) {
            return o << subtemp.getName() << ":\n    extension: " << subtemp.getExtension() << "\n    output: " << subtemp.getOutputPath();
          }
          inline const bool isEqual(const std::string& st) const { return name == st; }
      };

    private:
      std::vector<Subtemplate> subtemplates;
      std::string name;
    public:
      // CONSTRUCTORS
      /**
       * @param fn          Absolute Filepath of templates subfolder 
       *                    or name of template
       * @param template    What template to use
       */
      Template(std::string fn);
      const void applyScheme(const Scheme& scheme) ;


      // STATIC METHODS
      static std::vector<Template> Builder(std::string fp="");  
      static Template findTemplate(std::string templateName);


      // GETTERS
      const std::string getName() const { return name; }
      const Subtemplate getSubtemplate(std::string name) const;


      // HELPERS
      friend std::ostream& operator<<(std::ostream& o, const Template& tmplate) {
        o << tmplate.getName() << ":\n";
        for (const auto subtemp : tmplate.subtemplates) 
          o << "  " << subtemp << '\n';
        return o;
      }
  };

}

#endif
