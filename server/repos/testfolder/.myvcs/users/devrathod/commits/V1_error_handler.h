Hello world!
/*
 * error_handler.h
 *
 * The error handler function provides a utility in displaying error messages to the end user.
 * The different permissible error types are indicated in the error_type enum object.
 *
 *  Created on: 08-Dec-2022
 *      Author: Dev Rathod
 *  Email ID: dmrathod@ucdavis.edu
 */

#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

// The different types of error message that are to be printed in this program.
enum error_type {INVALID_INPUT_TYPE, INVALID_INPUT_PARAMETERS, INVALID_ROWS, INVALID_COLUMNS,
	INVALID_INPUT_ROW, INVALID_INPUT_COLUMN, INVALID_STRAIGHT_LINE , INVALID_CELL_NUMBER,
    INVALID_RESIZE_INPUT, INTERNAL_ERROR};

/*
 * This function is to print the help commands.
 * Arguments - None
 *
 * Returns - Void
*/
void print_help();

/*
 * This function is used to print the error statements
 * Arguments - the Error type enum variable
 *
 * Returns - Void
*/
void print_error(enum error_type error_code);


#endif /* ERROR_HANDLER_H_ */

// end of file
