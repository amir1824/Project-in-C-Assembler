/* Contains general-purposed functions, for both passes and many usages */
#ifndef _UTILS_H
#define _UTILS_H

#include "globals.h"


/** moves the index to the next place in string where the char isn't white */
#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

/*
 * Concatenates both string to a new allocated memory
 s0 The first string
 s1 The second string
  A pointer to the new, allocated string
 */
char *strallocat(char *s0, char* s1);

bool parse_symbol(line_info line, char *symbol_dest);

/*
  Returns the instruction enum by the instruction's name, without the opening '.'
  name The instruction name, without the '.'
  The instruction enum if found, NONE_INST if not found.
 */
instruction find_instruction_by_name(char *name);

/*
 * Returns whether the string is a valid 12-bit integer
 * @param string The number string
 * @return Whether a valid 12-bit signed integer.
 */
bool is_int(char* string);

/*
  Allocates memory in the required size. Exits the program if failed.
 size The size to allocate in bytes
 A generic pointer to the allocated memory if succeeded
 */
void *malloc_with_check(long size);

/*
 * Returns whether a label can be defined with the specified name.
 * @param name The label name
 * @return Whether the specified name is valid,
 */
bool is_valid_label_name(char* name);

/*
  Returns whether a string is alphanumeric.
  string The string
  Whether it's alphanumeric
 */
bool is_alphanumeric_str(char *string);

/*Returns TRUE if name is saved word*/
bool is_reserved_word(char *name);

/*
  Prints a detailed error message, including file name and line number by the specified message,
  formatted as specified in App. B of "The C Programming language" for printf.
  message The error message
  The arguments to format into the message
  printf result of the message
 */
int printf_line_error(line_info line, char *message, ...);

/* Frees all the allocated memory inside the code image */
void free_code_image(machine_word **code_image, long fic);

#endif
