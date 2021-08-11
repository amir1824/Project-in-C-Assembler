/* Constants */

#ifndef _GLOBALS_H
#define _GLOBALS_H

/** Boolean (t/f) definition */
typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

/** Maximum size of code image and data image */
#define CODE_ARR_IMG_LENGTH 600

/** Maximum length of a single source line  */
#define MAX_LINE_LENGTH 80

/** Initial IC value */
#define IC_INIT_VALUE 100

/* Note: many enum declaration contains NONE_X value - which is a flag for not found during parsing. */

/** Operand addressing type */
typedef enum addressing_types {
	/** Immediate addressing (0) */
	IMMEDIATE_ADDR = 0,
	/** Direct addressing (1) */
	DIRECT_ADDR = 1,
	/** Relative addressing (2) */
	RELATIVE_ADDR = 2,
	/** Register addressing */
	REGISTER_ADDR = 3,
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;

/** Commands opcode */
typedef enum opcodes {
	/* First Group */
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 2,

	LEA_OP = 4,
	/* END First Group */

	/* Second Group */
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,

	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,

	RED_OP = 12,
	PRN_OP = 13,
	/* END Second Group */

	/* Third Group */
	RTS_OP = 14,
	STOP_OP = 15,
	/* END Third Group */

	/** Failed/Error */
	NONE_OP = -1
} opcode;

/** Commands funct */
typedef enum funct {
	/* OPCODE 2 */
	ADD_FUNCT = 10,
	SUB_FUNCT = 11,

	/* OPCODE 5 */
	CLR_FUNCT = 10,
	NOT_FUNCT = 11,
	INC_FUNCT = 12,
	DEC_FUNCT = 13,

	/* OPCODE 9 */
	JMP_FUNCT = 10,
	BNE_FUNCT = 11,
	JSR_FUNCT = 12,

	/** Default (No need/Error) */
	NONE_FUNCT = 0
} funct;

/** Registers - r0->r1 + not found */
typedef enum registers {
	R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	NONE_REG = -1
} reg;
/** Represents a single code word */
typedef struct code_word {
	/* First byte: ARE+funct */
	
	unsigned int funct: 4;
	/* 2nd byte: destination+addressing, source */
	
	unsigned int dest_addressing: 2;
	
	/* Third byte: source addressing, opcode */
	unsigned int src_addressing: 2;
	unsigned int opcode: 4;

} code_word;

/** Represents a single data word. */
typedef struct data_word {
	int ARE: 12;
	/* The data content itself (a method for putting data into these field is defined) */
	unsigned long data;
} data_word;

/** Represents a general machine code word contents */
typedef struct machine_word {
	/* if it represents code (not additional data), this field contains the total length required by the code. if it's data, this field is 0. */
	long length;
	/* The content can be code or data */
	union word {
		data_word *data;
		code_word *code;
	} word;
} machine_word;

/** Instruction type (.data, .entry, etc.) */
typedef enum instruction {
	/** .data instruction */
	DATA_INST,
	/** .extern instruction */
	EXTERN_INST,
	/** .entry instruction */
	ENTRY_INST,
	/** .string instruction */
	STRING_INST,
	/** Not found */
	NONE_INST,
	/** Parsing/syntax error */
	ERROR_INST
} instruction;

/**
 * Represents a single source line, including it's details
 */
typedef struct line_info {
	/** Line number in file */
	long line_number;
	/** File name */
	char *file_name;
	/** Line content (source) */
	char *content;
} line_info;

#endif
