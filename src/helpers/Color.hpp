#include <iostream>
#include <exception>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <fmt/core.h>

struct rgb_base {
  virtual const std::string getstr() const { return "rgb base"; }
  template <typename T> static const std::string makestr(T r, T g, T b) {
    return "r="+std::to_string(r)+", g="+std::to_string(g)+", b="+std::to_string(b); 
  }
};

struct rgb_hex : public rgb_base {
  const std::string r;
  const std::string g;
  const std::string b;

  rgb_hex(const std::string r, const std::string g, const std::string b) : r(r), g(g), b(b) {}
  const std::string getstr() { return std::string("#")+r+g+b; }

};
struct rgb_dec : public rgb_base {
  const float r;
  const float g;
  const float b;

  rgb_dec(const float r, const float g, const float b) : r(r), g(g), b(b) {}
  const std::string getstr() { return makestr(r,g,b); }
};
struct rgb_int : public rgb_base {
  const ushort r;
  const ushort g;
  const ushort b;

  rgb_int(const ushort r, const ushort g, const ushort b) : r(r), g(g), b(b) {}
  const std::string getstr() { return makestr(r,g,b); }
};
struct hsl_dec : public rgb_base {
  const float h;
  const float s;
  const float l;

  hsl_dec(const float h, const float s, const float l) : h(h), s(s), l(l) {}
  const std::string getstr() { return makestr(h,s,l); }
};

class Color {
  // decimal val -> [0,1] float
  // integer val -> [0, 255] unsigned short
  // hexadec val -> [0x0, 0xFF] string
  private:
    ushort r, g, b;
    // helpers for conversions
    template<typename T>
    static std::string numtostring(T x) {
      return fmt::format("{:x}", x);
    }
  public:
    static ushort hextoint(const char* n) { return std::stoul(n, nullptr, 16); };
    static ushort hextoint(const std::string n) { return std::stoul(n, nullptr, 16); };
    static ushort dectoint(float n) { return n * 255; };
    static std::string inttohex(ushort n) { return numtostring(n); }
    static float inttodec(ushort n) { return static_cast<float>(n) / 255.0f; };
    
    const rgb_hex getHexValue() { return rgb_hex(inttohex(r), inttohex(g), inttohex(b)); }
    const rgb_dec getDecVal() { return rgb_dec(inttodec(r), inttodec(g), inttodec(b)); }
    const rgb_int getIntVal() { return rgb_int(r, g, b); }
    const hsl_dec getHSLVal() {
      rgb_dec dec = getDecVal();
      float H, S, L;
      // Find minimum and maximum values of R, G, and B
      std::pair<float, float> mm = std::minmax({dec.r, dec.g, dec.b});
      float min = mm.first;
      float max = mm.second;

      L = (min + max) / 2.0f;
      L = std::ceil(L * 100.0f) / 100.0f; // rounded up

      if (min == max) S = 0.0f;
      else if (L <= 0.5f) S = (max-min) / (max+min);
      else if (L > 0.5f) S = (max-min) / (2.0f-max-min);
      S = std::floor(S * 100.0f) / 100.0f; // rounded down
     
      // https://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/
      // TODO: finish H conversion
      if (max == dec.r) H = (dec.g - dec.b) / (max-min);
      else if (max == dec.g) H = 2.0f + (dec.b - dec.r) / (max-min);
      else H = 4.0f + (dec.r - dec.g) / (max-min);

      return hsl_dec(H, S, L);

    };

    // constructors
    Color(const char* r, const char* g, const char* b) : r(hextoint(r)), g(hextoint(g)), b(hextoint(b)) {}
    Color(const rgb_hex& hex) : r(hextoint(hex.r)), g(hextoint(hex.g)), b(hextoint(hex.b)) {}
    Color(const std::string& rgb) {
      std::string buf = rgb;
      if (buf.find('#') != std::string::npos) buf = buf.substr(1);
      if (buf.length() != 6) throw std::runtime_error("Invalid hextring: " + buf);
      this->r = hextoint(buf.substr(0,2).c_str());
      this->g = hextoint(buf.substr(2,2).c_str());
      this->b = hextoint(buf.substr(4,2).c_str());
    }

    Color(const float r, const float g, const float b) : r(dectoint(r)), g(dectoint(g)), b(dectoint(b)) {}
    Color(const rgb_dec& dec) : r(dectoint(dec.r)), g(dectoint(dec.g)), b(dectoint(dec.b)) {}

    Color(const ushort r, const ushort g, const ushort b) : r(r), g(g), b(b) {}
    Color(const rgb_int& intn) : r(intn.r), g(intn.g), b(intn.b) {}

    // operators
    bool operator==(const Color other) {
      std::cout << std::string("r[")+std::to_string(r)+"=="+std::to_string(other.r) +
        "], g["+std::to_string(g)+"=="+std::to_string(other.g)+ 
        "], b["+std::to_string(b)+"=="+std::to_string(other.b)+"]\n";
      return r==other.r && g==other.g && b==other.b;
    }
};
