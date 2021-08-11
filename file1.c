#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "file1.h"
#include "utils.h"


/**
 * Validates the operands addressing types, and prints error message if needed.
  line The current line information
  op1_addressing The current addressing of the first operand
  op2_addressing The current addressing of the second operand
  op1_valid_addr_count The count of valid addressing types for the first operand
   op2_valid_addr_count The count of valid addressing types for the first operand
  ... The valid addressing types for first & second operand, respectively
  Whether addressign types are valid
 */
static bool validate_op_addr(line_info line, addressing_type op1_addressing, addressing_type op2_addressing,
		int op1_valid_addr_count, int op2_valid_addr_count,...);


bool analyze_operands(line_info line, int i, char **destination, int *operand_count, char *c) {
	int j;
	*operand_count = 0;
	MOVE_TO_NOT_WHITE(line.content, i)
	if (line.content[i] == ',') {
		printf_line_error(line, "Unexpected comma after command.");
		return FALSE; /* an error occurred */
	}
	/* allocate space to save the processes operands */
	destination[0] = malloc_with_check(MAX_LINE_LENGTH);
	destination[1] = malloc_with_check(MAX_LINE_LENGTH);

	/* Until noy too many operands (max of 2) and it's not the end of the line */
	for (*operand_count = 0; line.content[i] != EOF && line.content[i] != '\n' && line.content[i];) {
		if (*operand_count == 2) /* =We already got 2 operands in, We're going ot get the third! */ {
			printf_line_error(line, "Too many operands for operation (got >%d)", *operand_count);
			free(destination[0]);
			free(destination[1]);
			return FALSE; /* an error occurred */
		}

		/* as long we're still on same operand */
		for (j = 0; line.content[i] && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != '\n' && line.content[i] != EOF &&
		            line.content[i] != ','; i++, j++) {
			destination[*operand_count][j] = line.content[i];
		}

		destination[*operand_count][j] = '\0';
		(*operand_count)++; /* We've just saved another operand! */
		MOVE_TO_NOT_WHITE(line.content, i)

		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i]) break;
		else if (line.content[i] != ',') {
			/* After operand & after white chars there's something that isn't ',' or end of line.. */
			printf_line_error(line, "Expecting ',' between operands");
			/* Release operands dynamically allocated memory */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
		i++;
		MOVE_TO_NOT_WHITE(line.content, i)
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i])
			printf_line_error(line, "Missing operand after comma.");
		else if (line.content[i] == ',') printf_line_error(line, "Multiple consecutive commas.");
		else continue; /* No errors, continue */
		{ /* Error found! (didn't continue) */
			/* No one forgot you two! */
			free(destination[0]);
			free(destination[1]);
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * A single lookup table element
 */
struct cmd_lookup_element {
	char *cmd;
	opcode op;
	funct fun;
};
/**
 * A lookup table for opcode & funct by command name
 */
static struct cmd_lookup_element lookup_table[] = {
		{"mov", MOV_OP, NONE_FUNCT},
		{"cmp",CMP_OP, NONE_FUNCT},
		{"add",ADD_OP, ADD_FUNCT},
		{"sub",SUB_OP, SUB_FUNCT},
		{"lea",LEA_OP, NONE_FUNCT},
		{"clr",CLR_OP, CLR_FUNCT},
		{"not",NOT_OP, NOT_FUNCT},
		{"inc",INC_OP, INC_FUNCT},
		{"dec",DEC_OP, DEC_FUNCT},
		{"jmp",JMP_OP, JMP_FUNCT},
		{"bne",BNE_OP, BNE_FUNCT},
		{"jsr",JSR_OP, JSR_FUNCT},
		{"red",RED_OP, NONE_FUNCT},
		{"prn",PRN_OP, NONE_FUNCT},
		{"rts",RTS_OP, NONE_FUNCT},
		{"stop",STOP_OP, NONE_FUNCT},
		{NULL, NONE_OP, NONE_FUNCT}
};
void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out) {
	struct cmd_lookup_element *e;
	*opcode_out = NONE_OP;
	*funct_out = NONE_FUNCT;
	/* iterate through the lookup table, if commands are same return the opcode of found. */
	for (e = lookup_table; e->cmd != NULL; e++) {
		if (strcmp(e->cmd, cmd) == 0) {
			*opcode_out = e->op;
			*funct_out = e->fun;
			return;
		}
	}
}

addressing_type get_addressing_type(char *operand) {
	/* if nothing, just return none */
	if (operand[0] == '\0') return NONE_ADDR;
	/* if first char is 'r', second is number in range 0-7 and third is end of string, it's a register */
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7' && operand[2] == '\0') return REGISTER_ADDR;
	/* if operand starts with # and a number right after that, it's immediately addressed */
	else if (operand[0] == '#' && is_int(operand + 1)) return IMMEDIATE_ADDR;
	/* if operand starts with & and has label afterwards, it;s realtively addressed */
	else if (operand[0] && operand[0] == '%' && is_valid_label_name(operand + 1)) return RELATIVE_ADDR;
	/* if operand is a valid label name, it's directly addressed */
	else if (is_valid_label_name(operand)) return DIRECT_ADDR;
	else return NONE_ADDR;
}

/**
 * Validates the operands' addressing types by the opcode of the instruction
  line The current source line info
  first_addressing The addressing of the first operand
  second_addressing The addressing of the second operand
  curr_opcode The opcode of the current instruction
 op_count The operand count of the current instruction
  Whether valid addressing
 */
bool validate_operand_by_opcode(line_info line, addressing_type first_addressing,
                                addressing_type second_addressing, opcode curr_opcode, int op_count) {
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) {
		/* 2 operands required */
		if (op_count != 2) {
			printf_line_error(line, "Operation requires 2 operands (got %d)", op_count);
			return FALSE;
		}
		/* validate operand addressing */
		if (curr_opcode == CMP_OP) {
			return validate_op_addr(line, first_addressing, second_addressing, 3, 3, IMMEDIATE_ADDR, DIRECT_ADDR,
			                            REGISTER_ADDR,
			                            IMMEDIATE_ADDR, DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == ADD_OP || curr_opcode == MOV_OP) { /* Also SUB_OP */
			return validate_op_addr(line, first_addressing, second_addressing, 3, 2, IMMEDIATE_ADDR, DIRECT_ADDR,
			                            REGISTER_ADDR,
			                            DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == LEA_OP) {

			return validate_op_addr(line, first_addressing, second_addressing, 1, 2, DIRECT_ADDR,
			                            DIRECT_ADDR, REGISTER_ADDR);
		}
	} else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		/* 1 operand required */
		if (op_count != 1) {
			if (op_count < 1) printf_line_error(line, "Operation requires 1 operand (got %d)", op_count);
			return FALSE;
		}
		/* validate operand addressing */
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP) { /* Also for NOT, INC, DEC */
			return validate_op_addr(line, first_addressing, NONE_ADDR, 2, 0, DIRECT_ADDR, REGISTER_ADDR);
		} else if (curr_opcode == JMP_OP) {/* Also for BNE,JSR */
			return validate_op_addr(line, first_addressing, NONE_ADDR, 2, 0, DIRECT_ADDR, RELATIVE_ADDR);
		} else { /* Then it's PRN */
			return validate_op_addr(line, first_addressing, NONE_ADDR, 3, 0, IMMEDIATE_ADDR, DIRECT_ADDR, REGISTER_ADDR);
		}
	} else if (curr_opcode <= STOP_OP && curr_opcode >= RTS_OP) {
		/* 0 operands exactly */
		if (op_count > 0) {
			printf_line_error(line, "Operation requires no operands (got %d)", op_count);
			return FALSE;
		}
	}
	return TRUE;
}


code_word *get_code_word(line_info line, opcode curr_opcode, funct curr_funct, int op_count, char *operands[2]) {
	code_word *codeword;
	addressing_type first_addressing = get_addressing_type(operands[0]);
	addressing_type second_addressing = get_addressing_type(operands[1]);
	/* validate operands by opcode - on failure exit */
	if (!validate_operand_by_opcode(line, first_addressing, second_addressing, curr_opcode, op_count)) {
		return NULL;
	}
	/* Create the code word by the data: */
	codeword = (code_word *) malloc_with_check(sizeof(code_word));

	codeword->opcode = curr_opcode;
	codeword->funct = curr_funct; /* if no funct, curr_funct = NONE_FUNCT = 0, and it should be the default. */
	
	
	codeword->dest_addressing = codeword->src_addressing = 0;
	/* Check if need to set the registers bits */
	if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) { /* First Group, two operands */
		codeword->src_addressing = first_addressing;
		codeword->dest_addressing = second_addressing;
		
		
	}
          else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) {
		codeword->dest_addressing = first_addressing;
		
	}
	return codeword;
}

static bool validate_op_addr(line_info line, addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count,
                      int op2_valid_addr_count, ...) {
	int i;
	bool is_valid;
	va_list list;

	addressing_type op1_valids[4], op2_valids[4];
	memset(op1_valids, NONE_ADDR, sizeof(op1_valids));
	memset(op2_valids, NONE_ADDR, sizeof(op2_valids));

	va_start(list, op2_valid_addr_count);
	/* get the variable args and put them in both arrays (op1_valids & op2_valids) */
	for (i = 0; i < op1_valid_addr_count && i <= 3 ;i++)
		op1_valids[i] = va_arg(list, int);
	for (; op1_valid_addr_count > 4; va_arg(list,
	                                        int), op1_valid_addr_count--); /* Go on with stack until got all  */
	/* Again for second operand by the count */
	for (i = 0; i < op2_valid_addr_count && i <= 3 ;i++)
		op2_valids[i] = va_arg(list, int);

	va_end(list);  /* We got all the arguments we wanted, goodbye va_arg */

	/* Make the validation itself: check if any of the operand addressing type has match to any of the valid ones: */
	is_valid = op1_valid_addr_count == 0 && op1_addressing == NONE_ADDR;
	for (i = 0; i < op1_valid_addr_count && !is_valid; i++) {
		if (op1_valids[i] == op1_addressing) {
			is_valid = TRUE;
		}
	}
	if (!is_valid) {
		printf_line_error(line, "Invalid addressing mode for first operand.");
		return FALSE;
	}
	/* Same */
	is_valid = op2_valid_addr_count == 0 && op2_addressing == NONE_ADDR;
	for (i = 0; i < op2_valid_addr_count && !is_valid; i++) {
		if (op2_valids[i] == op2_addressing) {
			is_valid = TRUE;
		}
	}
	if (!is_valid) {
		printf_line_error(line, "Invalid addressing mode for second operand.");
		return FALSE;
	}
	return TRUE;
}

reg get_register_by_name(char *name) {
	if (name[0] == 'r' && isdigit(name[1]) && name[2] == '\0') {
		int digit = name[1] - '0'; /* convert digit ascii char to actual single digit */
		if (digit >= 0 && digit <= 4) return digit;
	}
	return NONE_REG; /* No match */
}

data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol) {
	signed long mask; /* For bitwise operations for data conversion */
	unsigned long ARE = 12, mask_un; 
	data_word *dataword = malloc_with_check(sizeof(data_word));
 
	if (addressing == DIRECT_ADDR) {
		ARE = is_extern_symbol ? 1 : 2;
	}
	dataword->ARE = ARE; /* Set ARE field value */

	/* Now all left is to encode the data */
	mask = -1;
	mask_un = mask; 
	mask_un >>= 12; 
	dataword->data = mask_un & data; /* Now just the 12 lsb bits area left and assigned to data field. */
	return dataword;
}
