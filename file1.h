/* Contains function prototypes for code.c, which are used to parse and analyze code and build it to a word */
#ifndef _FILE1_H
#define _FILE1_H
#include "table.h"
#include "globals.h"

/*
  Detects the opcode and the funct of a command by it's name
  cmd The command name (string)
  opcode_out The opcode value destination
  funct_out The funct value destination
 */
void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out);

/*
  Returns the addressing type of an operand
  operand The operand's string
  The addressing type of the operand
 */
addressing_type get_addressing_type(char *operand);

/**
  Validates and Builds a code word by the opcode, funct, operand count and operand strings
  curr_opcode The current opcode
  curr_funct The current funct
 op_count The operands count
  operands a 2-cell array of pointers to first and second operands.
A pointer to code word struct, which represents the code. if validation fails, returns NULL.
 */
code_word *get_code_word(line_info line, opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]);

/**
  the register enum value by it's name
  name The name of the register
  The enum value of the register if found. otherwise, returns NONE_REG
 */
reg get_register_by_name(char *name);

/**
  Builds a data word by the operand's addressing type, value and whether the symbol (if it is one) is external.
  addressing The addressing type of the value
  data The value
  is_extern_symbol If the symbol is a label, and it's external
A pointer to the constructed data word for the data by the specified properties.
 */
data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol);

/**
 * Separates the operands from a certain index, puts each operand into the destination array,
 and puts the found operand count in operand count argument
  line The command text
  i The index to start analyzing from
  destination At least a 2-cell buffer of strings for the extracted operand strings
  operand_count The destination of the detected operands count
  command The current command string
  Whether analyzing succeeded
 */
bool analyze_operands(line_info line, int i, char **destination, int *operand_count, char *command);

#endif
