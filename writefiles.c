#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "table.h"

#define KEEP_ONLY_12_LSB(value) ((value) & 0xFFF)
/**
 * "Cuts" the msb of the value, keeping only it's lowest 12bits
 
 */


/*
 * Writes the code and data image into an .ob file, with lengths on top
 code_img The code image
  data_img The data image
  icf The final instruction counter
  dcf The final data counter
  filename The filename, without the extension
 Whether succeeded
 */
static int write_ob(machine_word **code_img, long *data_img, long icf, long dcf, char *filename);

/**
  a symbol table to a file. Each symbol and it's address in line, separated by a single space.
  tab The symbol table to write
  filename The filename without the extension
  file_extension The extension of the file, including dot before
  Whether succeeded
 */
static int write_table_to_file(table tab, char *filename, char *file_extension);

int write_output_files(machine_word **code_img, long *data_img, long icf, long dcf, char *filename,
                       table symbol_table) {
	/* Write .ob file */
	return write_ob(code_img, data_img, icf, dcf, filename) &&
	       /* Write *.ent and *.ext files: call with symbols from external references type or entry type only */
	       write_table_to_file(get_entries_by_type(symbol_table, EXTERNAL_REFERENCE), filename, ".ext") &&
	       write_table_to_file(get_entries_by_type(symbol_table, ENTRY_SYMBOL), filename, ".ent");
}

static int write_ob(machine_word **code_img, long *data_img, long icf, long dcf, char *filename) {
	int i;
	long val;
	FILE *file_desc;
	/* add extension of file to open */
	char *output_filename = strallocat(filename, ".ob");
	/* Try to open the file for writing */
	file_desc = fopen(output_filename, "w");
	free(output_filename);
	if (file_desc == NULL) {
		printf("Can't create or rewrite to file %s.", output_filename);
		return FALSE;
	}

	/* print data/code word count on top */
	fprintf(file_desc, "%ld %ld", icf - IC_INIT_VALUE, dcf);

	/* starting from index 0, not IC_INIT_VALUE as icf, so we have to subtract it. */
	for (i = 0; i < icf - IC_INIT_VALUE; i++) {
		if (code_img[i]->length > 0) {
			val = (code_img[i]->word.code->opcode << 8) | (code_img[i]->word.code->src_addressing << 2) |
			        (code_img[i]->word.code->dest_addressing << 0) |
			        (code_img[i]->word.code->funct) << 4 ;
			      
		} else {
			
			val = (KEEP_ONLY_12_LSB(code_img[i]->word.data->data) << 2); 
		}
		/* Write the value to the file - first */
		fprintf(file_desc, "\n%.4d %.3lx A ", i + 100, val);
	}

	/* Write data image. dcf starts at 0 so it's fine */
	for (i = 0; i < dcf; i++) {
		/* print only lower 12 bytes */
		val = KEEP_ONLY_12_LSB(data_img[i]);
		/* print at least 4 digits of hex, and 3 digits of dc */
		fprintf(file_desc, "\n%.4ld %.3lx E", icf + i, val);
	}

	/* Close the file */
	fclose(file_desc);
	return TRUE;
}

static int write_table_to_file(table tab, char *filename, char *file_extension) {
	FILE *file_desc;
	/* concatenate filename & extension, and open the file for writing: */
	char *full_filename = strallocat(filename, file_extension);
	file_desc = fopen(full_filename, "w");
	free(full_filename);
	/* if failed, print error and exit */
	if (file_desc == NULL) {
		printf("Can't create or rewrite to file %s.", full_filename);
		return FALSE;
	}
	/* if table is null, nothing to write */
	if (tab == NULL) return TRUE;

	/* Write first line without \n to avoid extraneous */
	fprintf(file_desc, "%s %.3ld ", tab->key, tab->value);
	while ((tab = tab->next) != NULL) {
		fprintf(file_desc, "\n%s %.3ld ", tab->key, tab->value);
	}
	fclose(file_desc);
	return TRUE;
}
