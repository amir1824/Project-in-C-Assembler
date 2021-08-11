#ifndef _FILE2_H
#define _FILE2_H
#include "globals.h"

/**
  Returns the first instruction detected from the index in the string.
  line The source string.
  index The index to start looking from.
  instruction_type indicates the detected instruction.
 */
instruction find_instruction_from_index(line_info line, int *index);

/**
 * Processes a .string instruction from index of source line.
  line The source line
  index The index
  data_img The current data image
  dc The current data counter
  Whether succeeded
 */
bool process_string_instruction(line_info line, int index, long *data_img, long *dc);

/**
  Processes a .data instruction from index of source line.
  line The source line
  index The index
  data_img The data image
  dc The current data counter
  Whether succeeded
 */
bool process_data_instruction(line_info line, int index, long *data_img, long *dc);

#endif
