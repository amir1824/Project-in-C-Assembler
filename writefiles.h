#ifndef _WRITEFILES_H
#define _WRITEFILES_H
#include "globals.h"
#include "table.h"

/*
  Writes the output files of a single assembled file
 code_img The code image
 data_img The data image
 icf The final instruction counter
 dcf The final data counter
 filename The filename (without the extension)
 ent_table The entries table
 ext_table The external references table
 Whether succeeded
 */
int write_output_files(machine_word **code_img, long *data_img, long icf, long dcf, char *filename,
                       table symbol_table);

#endif
