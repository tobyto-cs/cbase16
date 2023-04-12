# base16 builder make in cpp

follows https://github.com/chriskempson/base16/blob/main/builder.md conventions
- Accepts *scheme* data in YAML from Standard Input
- Accepts template data in mustache from `-t, --template`
- Output onto standard output

`cbbuilder`
Extra features
- Accepts template and scheme by installed name
  - Configurable install locations

- Inject `-i --inject` option to directly apply theme to file
  - If no files are specified, will inject into all configured files (from a list)
  - Config files (paths) are seperated by spaces

- `-s, --scheme` or `-t, --template` without any arguments will list all installed names
- `-o --output` will output into the specified file


TODO:
