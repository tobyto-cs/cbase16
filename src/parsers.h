#ifndef PARSERS
#define PARSERS

#include <iostream>
#include <fstream>

#define STARTSTR "##cbase16_start::"
#define ENDSTR "##cbase16_end##"

/**
 * Injects schema and template based on a "Start" and "End" string
 * STARTSTR = ##cbase16_start::<schema>:<template>##
 * ENDSTR = ##cbase16_end###
 * 
 * @param fn        Filepath of file to replace from
 * @param backup    If a backup is created locally (default=true)
 **/
void injectSection(std::string fn, bool backup=true);

#endif
