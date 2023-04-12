#include <actions/build.hpp>

namespace cbase
{
  vector<string> split_by_delim(const string& s, const string& delim)
  {
    vector<string> res;

    size_t cur_pos = 0, last_pos = 0;
    string token;
    while ((cur_pos = s.find(delim, last_pos)) != string::npos)
    {
      token = s.substr(last_pos, cur_pos-last_pos);
      last_pos = cur_pos+delim.length();
      res.push_back(token);
    }
    res.push_back(s.substr(last_pos));

    return res;
  }


  fs::path find_template(const string& tmp)
  {
    // tmp follows the schema:
    // - "<name>" - uses default subtemplate
    // - "<name>::<subtemplate>"
    if (tmp.empty()) throw std::runtime_error("Template given is and empty string");

    string delim = "::";
    vector<string> temp_name = split_by_delim(tmp, delim);
    string temp, sub_temp;

    temp = temp_name[0];

    if (temp_name.size() == 1) sub_temp = "default.mustache";

    if (temp_name.size() == 2) sub_temp = temp_name[1] + ".mustache";

    fs::path ret, cur_path;
    for (const auto& dir : fs::directory_iterator(CBASE_TEMPLATES_DIR))
    {
      cur_path = dir.path() / "templates";
      if (dir.path().filename() == temp)
      {
        // Template is found
        if (!fs::exists(cur_path / sub_temp))
          throw std::runtime_error(fmt::format("Subtemplate {} is not found in path {}", sub_temp, dir.path().string()));

        ret = cur_path / sub_temp;
      }
    }
    if (ret.empty()) throw std::runtime_error(fmt::format("Template {} is not found", temp));

    return ret;

  }

  int build_action(const po_struct& args)
  {
    Scheme scheme(args);

    find_template(args.tmplate->data());

    if (!args.output) std::cout << scheme.apply_template(find_template(args.tmplate->data())) << '\n';
    else
    {
      // TODO: Write to specified output location
    }

    return 0;
  }
}
