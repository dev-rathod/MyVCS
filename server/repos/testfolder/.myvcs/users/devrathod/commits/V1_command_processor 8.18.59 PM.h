Hello world!
/*
 * input_validator.h
 *
 * This program is used to process user commands / functions that alters the given matrix. The functions /
 * commands include:
 *   - quit (q)
 *   - help (h)
 *   - write (w start_row start_column end_row end_column)
 *   - erase (e num_rows num_cols)
 *   - resize (r num_rows num_cols)
 *   - add (a [r]/[c] position)
 *   - delete (d [r]/[c] position)
 *   - save (s file_name)
 *   - load (l file_name)
 * Check the HW9.1 Paint problem statement for details. The sample commands are mentioned in brackets (see above)
 *
 * Check comments in each function to see it behavior.
 *
 * Created on: 08-Dec-2022
 * Author: Dev Rathod
 * Email ID: dmrathod@ucdavis.edu
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_


#include "error_handler.h"
#include "screen_painter.h"

// struct variable to store various elements of the input command
struct input_cmd{
	char cmd; // The command type (e.g q / a / h / etc)
	int param1; // The first parameter
	int param2; // The second parameter
	int param3;  // The third parameter
	int param4; // The fourth parameter
	char *fileName; // The file name to be loaded / saved
};

// enum objects that shows the direction in which the matrix needs to be re-painted
// In contains various flags that indicate the above.
enum paint_direction {HORIZONTAL, VERTICAL, LEFT_DIAGONAL, RIGHT_DIAGONAL, ERROR};

// All permissible command types / functions that can be executed by this program
enum command_types {QUIT_CMD = 'q', HELP_CMD = 'h', WRITE_CMD = 'w',
	ERASE_CMD = 'e', RESIZE_CMD = 'r', ADD_CMD = 'a', DELETE_CMD = 'd',
	SAVE_CMD = 's', LOAD_CMD = 'l'};

/*
 * This function parses, validates and stores the input commands details in a struct variable.
 *
 * Arguments:
 *   - cmd - Character Pointer to location where command is stored
 *   - inputCmd - A blank command Struct variable which need to be loaded after the command is parsed,
 *                validated sucessfully.
 *   - matrix - the print matrix struct variable
 *
 * Returns:
 *   0 - In case of validation error
 *   1 - In case the syntax is correct and command variable is loaded successfully in struct
 */
int loadInputCommand(char *cmd, struct input_cmd *inputCmd, struct paint_matrix *matrix);

/*This function processes the input command and calls the specific business function for its execution.
*
* Arguments:
*	struct pointer that holds the details of iput command
*	struct pointer that holds the paint matrix
*
* Return Value: Updated struct paint matrix variable
*/
struct paint_matrix* processCommand(struct input_cmd *inputCmd, struct paint_matrix *matrix);

/*This function is used to identify which direction of updating the paint matrix variable.
* Arguments:-
*	- row co-ordinate 1
*	- row co-ordinate 2
*	- column co-ordinate 1
*	- column co-ordinate 2
*
* Return Value: enum of direction
*/
enum paint_direction getWriteDirection(int x1, int y1, int x2, int y2);

/* This function is used to convert an character array into its integer value.
*  Arguments:-
*	Character input.
*
*  Returns value : The converted integer. In case of error, it returns -1.
*/
int convertAsciiToNumber(char * input);

/*
 * This function deletes either a row or column from the matrix.
 * Arguments:-
 *	- Struct paint matrix.
 *	- rows and column indicator / flag.
 *	- target to delete the rows and columns.
 *
 * Return Value: Struct.It returns NULL if program fails to re-allocate memory
*/
struct paint_matrix* deletePaintMatrix(struct paint_matrix *matrix, int intrc, int target);

/*This function is used to add row or column to a matrix
* Arguments:
*	- Paint struct matrix
*	- row or column indicator
*	- the row location which needs to be added
*
* Return Value: Updated Struct. It returns NULL in case there is failure in re-allocating memory.
*/
struct paint_matrix* addPaintMatrix(struct paint_matrix *matrix, int intrc, int target);

/*This function re-sizes the array to the new value specified in arguments
* Argument:-
*	- Paint struct matrix.
*	- new number of rows.
*	- new number of cols.
*
* Return Value: Update struct.It returns NULL in case the program failed re-allocating memory
*
*/
struct paint_matrix* resizePaintMatrix(struct paint_matrix *matrix, int newRows, int newCols);

/*This function erases a cell from the matrix.
*Argument:-
*	- paint struct matrix.
*	- row of cell.
*	- column of cell.
*
* Return Value: void.
*/
void eraseCell(struct paint_matrix *matrix, int row, int col);

/*This function is invoked when we need to write a Right Diagonal on the matrix using recursion.
* Argument:-
*	- Paint struct matrix.
*	- start row
*	- end rows
*	- start column
*	- end column
*
* Return Value: void.
*/
void writeRightDiagonal(struct paint_matrix *matrix, int r1, int r2, int c1, int c2);

/*This function is invoked when we need to write a Left Diagonal on the matrix using recursion.
* Argument:-
*	- Paint struct matrix.
*	- start row
*	- end rows
*	- start column
*	- end column
*
* Return Value: void.
*/
void writeLeftDiagonal(struct paint_matrix *matrix, int r1, int r2, int c1, int c2);

/*This function is invoked when we need to write a Vertical line on the matrix using recursion.
* Argument:-
*	- Paint struct matrix.
*	- start row
*	- end rows
*	- start column
*	- end column
*
* Return Value: void.
*/
void writeVertical (struct paint_matrix *matrix, int col, int r1, int r2);

/*This function is invoked when we need to write a Horizontal line on the matrix using recursion.
* Argument:-
*	- Paint struct matrix.
*	- start row
*	- end rows
*	- start column
*	- end column
*
* Return Value: void.
*/
void writeHorizontal (struct paint_matrix *matrix, int row, int c1, int c2);

/*Saves the current matrix into a custom file name specified by the user, which can be later used to load it back.
* Arguments:
*	Paint matrix struct.
*	The name of the file.
*
* Returns - Void
*/
void savePaintMatrix(struct paint_matrix *matrix, char *fileName);

/*
* This function loads the existing paint matrix from the saved file and reallocates memory to exiting paint matrix variable
* It updates the size (num_rows and num_cols) of the matrix by reading the first line to digits(rows and columns)..
*
* Arguments:
*	painter struct paint matrix.
*	file name to be opened.
*
* Return Value: Updated struct paint matrix variable. It returns NULL in case there was an error re-allocating
*   memory.
*/
struct paint_matrix* loadPaintMatrix(struct paint_matrix *matrix, char * fileName);


#endif /* COMMAND_PROCESSOR_H_ */

// end of file
