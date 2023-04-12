#include <scheme/scheme.hpp>

namespace cbase {

const set<string> Scheme::validTags = {
  "scheme", "author", "base00", "base01", "base02", "base03", "base04",
  "base04", "base05", "base06", "base07", "base08", "base09",
  "base0A", "base0B", "base0C", "base0D", "base0E", "base0F"
};




Scheme::Scheme(const po_struct& args)
{
  vector<char> data_buf;

  if (!args.scheme)
  {
    // Piped from STDIN
    string line;
    while (std::getline(std::cin, line))
    {
      data_buf.insert(data_buf.end(), line.begin(), line.end());
      data_buf.push_back('\n');
    }

  }
  else
  {
    // Read from a file
    fs::path fp(args.scheme.get());

    if (fp.empty()) throw fs::filesystem_error("Scheme filepath is empty", fp, error_code());
    if (!fs::exists(fp)) throw fs::filesystem_error("Scheme file does not exist", fp, error_code());

    ifstream file;
    file.exceptions(std::ios_base::badbit);
    file.open(fp);
    data_buf.insert(data_buf.begin(), std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
  }

  if (args.debug) for (char c : data_buf) std::cout << c;

  // Parse the YAML
  ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(data_buf));
  vector<ryml::substr> keys;
  string keyBuf, valBuf;
  size_t numValidTags = 0;

  for (ryml::NodeRef n : tree.rootref().children())
  {
    c4::from_chars(n.key(), &keyBuf);
    if (!is_valid_tag(keyBuf)) throw std::runtime_error("Scheme contains invalid key: " + keyBuf + '\n');
    if (!n.has_val()) throw std::runtime_error("Scheme is invalid, tag " + keyBuf + " has no value\n");
    numValidTags++;
    c4::from_chars(n.val(), &valBuf);
    tags.insert(pair<string, string>(keyBuf, valBuf));
  }

  /* for (auto p : tags) fmt::print("{}: {}\n", p.first, p.second); */

  if (numValidTags != validTags.size())
  {
    stringstream err;
    err << "Scheme doesn not contain all the required tags, " << numValidTags << "/" << validTags.size() << '\n';
    throw std::runtime_error(err.str());
  }
}


const string Scheme::get_tag(const string& tag)
{
  auto it = tags.find(tag);
  if (it != tags.end()) return it->second;
  else return string();
}

mstch::map Scheme::build_map()&
{
  typedef pair<const string, mstch::node> map_pair;
  mstch::map mp;

  mp.insert(map_pair("scheme-name", get_tag("name")));
  mp.insert(map_pair("scheme-author", get_tag("name")));

  string slug_name(get_tag("name"));
  std::replace(slug_name.begin(), slug_name.end(), ' ', '-');
  mp.insert(map_pair("scheme-slug", slug_name));

  string base_num, base_val, hex_digit;
  for (ushort i=0; i<16; i++)
  {
    hex_digit = Color::inttohex(i);
    for (auto &c : hex_digit) c = toupper(c);
    base_num = "base0" + hex_digit;
    base_val = get_tag(base_num);

    Color clr = Color(base_val);
    rgb_hex clr_hex = clr.getHexValue(); // 00-FF        char*
    rgb_dec clr_dec = clr.getDecVal();   // 0.0f - 1.0f  float
    rgb_int clr_int = clr.getIntVal();   // 0 - 255      ushort
    hsl_dec clr_hsl = clr.getHSLVal();

    // -hex
    mp.insert(map_pair(std::string(base_num+"-hex"), base_val));
    // -hex-bgr
    std::string rev_val = base_val;
    reverse(rev_val.begin(), rev_val.end());
    mp.insert(map_pair(std::string(base_num+"-hex-bgr"), rev_val));
    // -hex-r
    mp.insert(map_pair(std::string(base_num+"-hex-r"), clr_hex.r));
    // -hex-g
    mp.insert(map_pair(std::string(base_num+"-hex-g"), clr_hex.g));
    // -hex-b
    mp.insert(map_pair(std::string(base_num+"-hex-b"), clr_hex.b));
    // -rgb-r
    mp.insert(map_pair(std::string(base_num+"-rgb-r"), clr_int.r));
    // -rgb-g
    mp.insert(map_pair(std::string(base_num+"-rgb-g"), clr_int.g));
    // -rgb-b
    mp.insert(map_pair(std::string(base_num+"-rgb-b"), clr_int.b));
    // -dec-r
    mp.insert(map_pair(std::string(base_num+"-dec-r"), clr_dec.r));
    // -dec-g
    mp.insert(map_pair(std::string(base_num+"-dec-g"), clr_dec.g));
    // -dec-b
    mp.insert(map_pair(std::string(base_num+"-dec-b"), clr_dec.b));
    // -hsl-h
    mp.insert(map_pair(std::string(base_num+"-hsl-h"), clr_hsl.h));
    // -hsl-s
    mp.insert(map_pair(std::string(base_num+"-hsl-s"), clr_hsl.s));
    // -hsl-l
    mp.insert(map_pair(std::string(base_num+"-hsl-l"), clr_hsl.l));
  }
   return mp;
}


// Requires a template as argument
// Template is a filepath to the .mustache file
const string Scheme::apply_template(const fs::path& fp)
{
  string data_buf;
  if (fp.empty()) throw fs::filesystem_error("Scheme filepath is empty", fp, error_code());
  if (!fs::exists(fp)) throw fs::filesystem_error("Scheme file does not exist", fp, error_code());

  ifstream file;
  file.exceptions(std::ios_base::badbit);
  file.open(fp);
  data_buf.insert(data_buf.begin(), std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

  fmt::print("TEMPLATE DATA_BUF: {}\n", data_buf);

  return apply_template(data_buf);
}
const string Scheme::apply_template(const string& buf)
{
  mstch::map context = build_map();
  return mstch::render(buf, context);
}

}
