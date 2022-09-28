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
#include "helpers/cbase_path.hpp"
#include "exceptions.hpp"
/**
 * A class that stores a templates colors
 */
namespace cbase {
  class Template {
    public: 

      // ---- SUBCLASS ----
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
      // ---- SUBCLASS::END ----


    private:
      std::vector<std::shared_ptr<Subtemplate>> subtemplates;
      std::string name;

    public:
      typedef std::unique_ptr<Template> ptr;
      typedef std::shared_ptr<Subtemplate> sub_ptr;
      // CONSTRUCTORS
      /**
       * @param fn          Absolute Filepath of templates subfolder 
       *                    or name of template
       * @param template    What template to use
       */
      Template(std::string fn);



      const void applyScheme(const Scheme& scheme) ;


      // STATIC METHODS
      /*
       * @param fp      If entered, filepath to search for valid templates
       *
       * @return        A list of strings of templates (NOT GUARENTEED TO BE VALID)     
       */
      static std::vector<std::string> StringList(std::string fp="");
      /*
       * @param fp      If entered, filepath to search for valid templates
       *
       * @return        A list of paths to the template (NOT GUARENTEED TO BE VALID)
       */
      static std::vector<std::filesystem::path> PathList(std::string fp="");
      /*
       * @param fp      If entered, filepath to search for valid templates
       *
       * @return        A vector of VALID template classes
       */
      static std::vector<ptr> Builder(std::string fp="");  
      /*
       * @param templateName    The string representation of a template to search for
       *
       * @return                The first template match, will return NULL if one is not found
       */
      static ptr findTemplate(std::string templateName);


      // GETTERS
      const std::string getName() const { return name; }
      const sub_ptr getSubtemplate(std::string name) const;


      // HELPERS
      friend std::ostream& operator<<(std::ostream& o, const Template& tmplate) {
        o << tmplate.getName() << ":\n";
        for (const auto subtemp : tmplate.subtemplates) 
          o << "  " << *subtemp << '\n';
        return o;
      }
  };

}

#endif
