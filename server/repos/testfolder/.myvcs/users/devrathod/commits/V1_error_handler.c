Hello world!
/*
 * error_handler.c
 *
 * The error handler function provides a utility in displaying error messages to the end user.
 * The different permissible error types are indicated in the error_type enum object.
 *
 *  Created on: 08-Dec-2022
 *      Author: Dev Rathod
 *  Email ID: dmrathod@ucdavis.edu
 */

#include "stdio.h"
#include "error_handler.h"


/*
 * This function is to print the help commands.
 * Arguments - None
 *
 * Returns - Void
*/
void print_help() {
	printf("Commands:\n");
	printf("Help: h\n");
	printf("Quit: q\n");
	printf("Draw line: w row_start col_start row_end col_end\n");
	printf("Resize: r num_rows num_cols\n");
	printf("Add row or column: a [r | c] pos\n");
	printf("Delete row or column: d [r | c] pos\n");
	printf("Erase: e row col\n");
	printf("Save: s file_name\n");
	printf("Load: l file_name\n");
}

/*
 * This function is used to print the error statements
 * Arguments - the Error type enum variable
 *
 * Returns - Void
*/
void print_error(enum error_type type) {
	switch(type) {
	case INVALID_INPUT_PARAMETERS:
		printf("Wrong number of command line arguments entered.\n");
		printf("Usage: ./paint.out [num_rows num_cols]\n");
		break;
	case INVALID_INPUT_TYPE:
		printf("Unrecognized command. Type h for help.\n");
		break;
	case INVALID_ROWS:
		printf("The number of rows is not an integer.\n");
		printf("The number of rows is less than 1.\n");
		break;
	case INVALID_COLUMNS:
		printf("The number of columns is not an integer.\n");
		printf("The number of columns is less than 1.\n");
		break;
	case INVALID_INPUT_ROW:
		printf("The row entered is invalid or greater than maximum row size.\n");
		printf("Please enter row within paint row size.\n");
		break;
	case INVALID_INPUT_COLUMN:
		printf("The column entered is invalid or greater than maximum column size.\n");
		printf("Please enter column within paint column size.\n");
		break;
	case INVALID_STRAIGHT_LINE:
		printf("The coordinates entered do not form a straight line.\n");
		printf("Please enter coordinates that form a straight line.\n");
		break;
	case INVALID_CELL_NUMBER:
		printf("The cell number entered is invalid.\n");
		printf("Please enter valid cell number.\n");
		break;
	case INVALID_RESIZE_INPUT:
		printf("Minimum resized row or column has to be at least 1 each.\n");
		break;
	case INTERNAL_ERROR:
		printf("An internal error has occurred and command could not be executed.\n");
		printf("Please re-run the program.\n");
		break;
	default:
		break;
	}
}

// end of file
