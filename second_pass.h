
#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H
#include "globals.h"
#include "table.h"

/*
 Processes a single source line in the second pass
 line The source line
 ic A pointer to the current instruction counter
 code_img The code image
 symbol_table The symbol table
 Whether succeeded
 */
bool process_line_spass(line_info line, long *ic, machine_word **code_img, table *symbol_table);

/*
 * Adds the symbol-dependent data words by the code line.
 line The source code line
  ic A pointer to the current instruction counter
  code_img The code image
  symbol_table The symbol table
  Whether succeeded
 */
bool add_symbols_to_code(line_info line, long *ic, machine_word **code_img, table *symbol_table);

#endif
