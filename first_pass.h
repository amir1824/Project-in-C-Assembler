#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H
/* Processes a code line in first pass */
#include "globals.h"

/**
  Processes a single line in the first pass
 line The line text
 datas The data symbol table
  codes The code symbol table
 externals The externals symbol table
  IC A pointer to the current instruction counter
  DC A pointer to the current data counter
  code_img The code image array
  data_img The data image array
  Whether succeeded.
 */
bool process_line_fpass(line_info line, long *IC, long *DC, machine_word **code_img, long *data_img,
                        table *symbol_table);

#endif
