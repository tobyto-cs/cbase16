#ifndef CBASE_BUILD
#define CBASE_BUILD

#include <string>
#include <ostream>
#include <vector>
#include <filesystem>
#include <sstream>

#include <mstch/mstch.hpp>

#include "../scheme.h"
#include "../template.h"
#include "../helpers/Color.hpp"

namespace fs = std::filesystem;

namespace cbase {
  using Subtemplate = Template::Subtemplate;

  mstch::map build_map(const std::shared_ptr<Scheme>& schm) {
    typedef std::pair<const std::string, mstch::node> map_pair;
    mstch::map map;

    map.insert(map_pair("scheme-name", schm->getTag("name")));
    map.insert(map_pair("scheme-author", schm->getTag("author")));

    std::string slug_name = schm->getTag("name");
    std::replace(slug_name.begin(), slug_name.end(), ' ', '-');
    map.insert(map_pair("scheme-slug", slug_name));

    std::string base_num, base_val, hex_digit;
    for (ushort i=0; i<16; i++) {
      hex_digit = Color::inttohex(i);
      for (auto &c : hex_digit) c = toupper(c);
      base_num = "base0"+hex_digit;
      base_val = schm->getTag(base_num);

      Color clr = Color(base_val);
      rgb_hex clr_hex = clr.getHexValue(); // 00-FF        char*
      rgb_dec clr_dec = clr.getDecVal();   // 0.0f - 1.0f  float
      rgb_int clr_int = clr.getIntVal();   // 0 - 255      ushort
      hsl_dec clr_hsl = clr.getHSLVal();

      // -hex
      map.insert(map_pair(std::string(base_num+"-hex"), base_val));
      // -hex-bgr
      std::string rev_val = base_val;
      reverse(rev_val.begin(), rev_val.end());
      map.insert(map_pair(std::string(base_num+"-hex-bgr"), rev_val));
      // -hex-r
      map.insert(map_pair(std::string(base_num+"-hex-r"), clr_hex.r)); 
      // -hex-g
      map.insert(map_pair(std::string(base_num+"-hex-g"), clr_hex.g));
      // -hex-b
      map.insert(map_pair(std::string(base_num+"-hex-b"), clr_hex.b));
      // -rgb-r
      map.insert(map_pair(std::string(base_num+"-rgb-r"), clr_int.r));
      // -rgb-g
      map.insert(map_pair(std::string(base_num+"-rgb-g"), clr_int.g));
      // -rgb-b
      map.insert(map_pair(std::string(base_num+"-rgb-b"), clr_int.b));
      // -dec-r
      map.insert(map_pair(std::string(base_num+"-dec-r"), clr_dec.r));
      // -dec-g
      map.insert(map_pair(std::string(base_num+"-dec-g"), clr_dec.g));
      // -dec-b
      map.insert(map_pair(std::string(base_num+"-dec-b"), clr_dec.b));
      // -hsl-h
      map.insert(map_pair(std::string(base_num+"-hsl-h"), clr_hsl.h));
      // -hsl-s
      map.insert(map_pair(std::string(base_num+"-hsl-s"), clr_hsl.s));
      // -hsl-l
      map.insert(map_pair(std::string(base_num+"-hsl-l"), clr_hsl.l));
    }

    return map;
  } 

  std::string build_template(const std::shared_ptr<Scheme>& scheme, 
    const std::shared_ptr<Template::Subtemplate>& sub_tmplate) {
    std::ifstream t(sub_tmplate->getMustache());
    std::stringstream buffer;
    buffer << t.rdbuf();

    mstch::map context = build_map(scheme);
    return mstch::render(buffer.str(), context);
}

  /*
   * @param scheme      stdin/name/path of scheme to use
   * @param tmplate     name/path of the template to use
   * @param sub_tmplate name/default of the subtemplate to use
   * @param output      stdout/default/path to output into
   *
   * @returns           result code
   */
  int build_action(const std::string& scheme, const std::string& tmplate, const std::string& sub_tmplate,  const std::string& output) {
    try {
      // Build Scheme from stream or name/filepath
      std::shared_ptr<Scheme> sch;
      if (scheme == STDIN) {
        if (isatty(STDIN_FILENO) == 1) throw std::runtime_error("Please pipe data into builder...");
        sch = std::make_shared<Scheme>(std::cin);
      }
      else sch = Scheme::findScheme(scheme);
      if (sch == nullptr) throw invalid_scheme(scheme);
      // Build Template and Subtemplate
      std::shared_ptr<Template> tem = Template::findTemplate(tmplate);
      if (tem == nullptr) throw invalid_template(tmplate);
      const std::shared_ptr<Subtemplate> sub_tem = tem->getSubtemplate(sub_tmplate);

      std::string built = build_template(sch, sub_tem);

      if (output == STDOUT) {
        std::cout << built;
      }
      else {
        fs::path output_dir;
        if (output == "default") output_dir = sub_tem->getOutputPath();
        else output_dir = fs::path(output);

        if (output_dir.empty() || !fs::exists(output_dir)) throw std::runtime_error("Invalid output path: " + output_dir.string());
        std::string file_name = "cbase16-"+sch->getTag("scheme")+sub_tem->getExtension();
        std::ofstream output_file;
        output_file.open(output_dir/file_name, std::ios::trunc);
        output_file << built;
        output_file.close();
      }
      return EXIT_SUCCESS;
    } catch (const invalid_obj& e) {
      std::cerr << e.what() << '\n';
      return EXIT_FAILURE;
    } catch (const std::runtime_error e) {
      std::cerr << e.what() << '\n';
      return EXIT_FAILURE;
    }
    std::cerr << "build_action I'm lost, im not supposed to be here";
    return EXIT_FAILURE;
  }
}

#endif
