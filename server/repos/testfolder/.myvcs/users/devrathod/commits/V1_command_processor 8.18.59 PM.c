Hello world!
/* command_processor.c
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

#include "command_processor.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

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
int loadInputCommand(char *cmd, struct input_cmd *inputCmd, struct paint_matrix *matrix) {

	int returnValue = 1;
	char cmdType[2] = {0};

	// Split the input command by considering SPACE as delimiter.
	char* token = strtok(cmd, " ");
	int intValue = -1;
	int elementNo = 0;
	while ( token != NULL) {
		// validate every token towards it correctness against the syntax and load the
		// input command struct object accordingly.
		printf("elementNo is :%d: and token is :%s:\n", elementNo, token);
		switch(elementNo) {
		case 0:
			strncpy(cmdType, token, sizeof(cmdType));
			cmdType[0] = tolower(cmdType[0]);
			if( (cmdType[0] != QUIT_CMD) && (cmdType[0] != HELP_CMD) && (cmdType[0] != WRITE_CMD) &&
					(cmdType[0] != ERASE_CMD) && (cmdType[0] != RESIZE_CMD) && (cmdType[0] != ADD_CMD) &&
					(cmdType[0] != DELETE_CMD) && (cmdType[0] != SAVE_CMD) && (cmdType[0] != LOAD_CMD)) {
				print_error(INVALID_INPUT_TYPE);
				return 0;
			}
			inputCmd->cmd = cmdType[0];
			break;
		case 1:
			if( (cmdType[0] != WRITE_CMD) &&
					(cmdType[0] != ERASE_CMD) && (cmdType[0] != RESIZE_CMD) && (cmdType[0] != ADD_CMD) &&
					(cmdType[0] != DELETE_CMD) && (cmdType[0] != SAVE_CMD) && (cmdType[0] != LOAD_CMD)) {
				print_error(INVALID_INPUT_TYPE);
				return 0;
			}

			if((cmdType[0] == WRITE_CMD) || (cmdType[0] == ERASE_CMD)) {
				intValue = convertAsciiToNumber(token);
				if((intValue >= matrix->rows) || (intValue == -1))  {
					print_error(INVALID_INPUT_ROW);
					return 0;
				} else
					inputCmd->param1 = intValue;
			}

			if(cmdType[0] == RESIZE_CMD) {
				intValue = convertAsciiToNumber(token);
				if(intValue == -1)  {
					print_error(INVALID_INPUT_ROW);
					return 0;
				} else {
					if(intValue > 0)
						inputCmd->param1 = intValue;
					else {
						print_error(INVALID_RESIZE_INPUT);
						return 0;
					}
				}
			}
			if((cmdType[0] == ADD_CMD) || (cmdType[0] == DELETE_CMD)) {
				if((strncmp(token, "r",1) == 0) || (strncmp(token, "c", 1) == 0)) {
					inputCmd->param1 = token[0];
				} else {
					print_error(INVALID_INPUT_TYPE);
					return 0;
				}
			}
			if((cmdType[0] == SAVE_CMD) || (cmdType[0] == LOAD_CMD)) {
				inputCmd->fileName = (char *)malloc(strlen(token) * sizeof(char));
				strcpy(inputCmd->fileName, token);
			}

			break;
		case 2:
			/*if( (cmdType[0] != WRITE_CMD) &&
					(cmdType[0] != ERASE_CMD) && (cmdType[0] != RESIZE_CMD) && (cmdType[0] != ADD_CMD) &&
					(cmdType[0] != DELETE_CMD) ) {
				print_error(INVALID_INPUT_TYPE);
				return 0;
			} */
			if((cmdType[0] == WRITE_CMD) || (cmdType[0] == ERASE_CMD)) {
				intValue = convertAsciiToNumber(token);
				if((intValue >= matrix->cols) || (intValue == -1)) {
					print_error(INVALID_INPUT_COLUMN);
					return 0;
				} else
					inputCmd->param2 = intValue;
			}
			if(cmdType[0] == RESIZE_CMD) {
				intValue = convertAsciiToNumber(token);
				if(intValue == -1)  {
					print_error(INVALID_INPUT_COLUMN);
					return 0;
				} else {
					if(intValue > 0)
						inputCmd->param2 = intValue;
					else {
						print_error(INVALID_RESIZE_INPUT);
						return 0;
					}
				}
			}
			if((cmdType[0] == ADD_CMD) || (cmdType[0] == DELETE_CMD)) {
				intValue = convertAsciiToNumber(token);
				if(inputCmd->param1 == 'r')  {
					if((intValue >= matrix->rows) || (intValue == -1)) {
						print_error(INVALID_INPUT_ROW);
						return 0;
					} else
						inputCmd->param2 = intValue;
				} else {
					if(inputCmd->param1 == 'c')  {
						if((intValue >= matrix->cols) || (intValue == -1)) {
							print_error(INVALID_INPUT_COLUMN);
							return 0;
						} else
							inputCmd->param2 = intValue;
					} else {
						print_error(INVALID_INPUT_TYPE);
						return 0;
					}
				}
			}
			break;
		case 3:
			/*if(cmdType[0] != WRITE_CMD) {
				print_error(INVALID_INPUT_TYPE);
				return 0;
			} */
			if(cmdType[0] == WRITE_CMD) {
				intValue = convertAsciiToNumber(token);
				if((intValue >= matrix->rows) || (intValue == -1)) {
					print_error(INVALID_INPUT_ROW);
					return 0;
				} else
					inputCmd->param3 = intValue;
			}
			break;
		case 4:
			/*if(cmdType[0] != WRITE_CMD) {
				print_error(INVALID_INPUT_TYPE);
				return 0;
			} */
			if(cmdType[0] == WRITE_CMD) {
				intValue = convertAsciiToNumber(token);
				if((intValue >= matrix->cols) || (intValue == -1)) {
					print_error(INVALID_INPUT_COLUMN);
					return 0;
				} else
					inputCmd->param4 = intValue;
			}
			break;
		default:
			print_error(INVALID_INPUT_TYPE);
			return 0;
		}

		token = strtok(NULL, " ");
		elementNo++;
	}

	return returnValue;
}

/* This function is used to convert an character array into its integer value.
*  Arguments:-
*	Character input.
*
*  Returns value : The converted integer. In case of error, it returns -1.
*/
int convertAsciiToNumber(char * input) {
	//Used to store the ascii return value.
	int returnValue = -1;

	//Converting the input to an integer.
	returnValue = atoi(input);

	//To check if the returnValure is null.
	if((returnValue == 0) && (strncmp(input, "0", 1) != 0))
		return -1;
	else
		return returnValue;
}

/*This function processes the input command and calls the specific business function for its execution.
*
* Arguments:
*	struct pointer that holds the details of iput command
*	struct pointer that holds the paint matrix
*
* Return Value: Updated struct paint matrix variable
*/
struct paint_matrix* processCommand(struct input_cmd *inputCmd, struct paint_matrix *matrix) {
	// This variable stores the direction value, if the user wants to write.
	enum paint_direction paintDirection;
	//This switch case consist of various commmands.
	//When it finds a match, it executes the command by calling the respective function.
	switch(inputCmd->cmd) {
	//If the user wants enters the help command it redirects to the print_help() function.
	case HELP_CMD:
		print_help();
		break;
	//If the user wants to write the matrix, it first computes the direction based on input parameters.
	//It matches the direction and then call a specific function to compute the end result.
	case WRITE_CMD:
		paintDirection = getWriteDirection(inputCmd->param1, inputCmd->param2, inputCmd->param3, inputCmd->param4);
		if(paintDirection == HORIZONTAL)
			writeHorizontal(matrix, inputCmd->param1, inputCmd->param2, inputCmd->param4);
		if(paintDirection == VERTICAL)
			writeVertical(matrix, inputCmd->param2, inputCmd->param1, inputCmd->param3);
		if(paintDirection == RIGHT_DIAGONAL)
			writeRightDiagonal(matrix, inputCmd->param1, inputCmd->param3, inputCmd->param2, inputCmd->param4);
		if(paintDirection == LEFT_DIAGONAL)
			writeLeftDiagonal(matrix, inputCmd->param1, inputCmd->param3, inputCmd->param2, inputCmd->param4);
		break;
	//If the user desires to erase a certain row or column, it call the eraseCell function.
	case ERASE_CMD:
		eraseCell(matrix, inputCmd->param1, inputCmd->param2);
		break;
	//If the user want to resize the matrix this function is called.
	case RESIZE_CMD:
		return resizePaintMatrix(matrix, inputCmd->param1, inputCmd->param2);
	//If the user want to add a row/column to the matrix.
	case ADD_CMD:
		return addPaintMatrix(matrix, inputCmd->param1, inputCmd->param2);
	//If the user wants to delete a rows/column.
	case DELETE_CMD:
		return deletePaintMatrix(matrix, inputCmd->param1, inputCmd->param2);
	//If the user wants to save the current canvas.
	case SAVE_CMD:
		savePaintMatrix(matrix, inputCmd->fileName);
		free(inputCmd->fileName);
		break;
	//This case is used to load the saved canvas.
	case LOAD_CMD:
		matrix = loadPaintMatrix(matrix, inputCmd->fileName);
		free(inputCmd->fileName);
		break;
	default:
		break;
	}

	return matrix;
}

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
struct paint_matrix* loadPaintMatrix(struct paint_matrix *matrix, char * fileName) {

	//Creates a file variable that opens the file.
	FILE *matrixInputFile = fopen(fileName, "r");
	//Array that reads the characters from the file.
	char line[500] = {0};
	//Defines what current line number are we on.
	int lineNo = 0;
	//Stores the new rows and columns from the file.
	int rows, cols;
	//Read each character from the file and processes it into storing it into the file.
	while(fgets(line, sizeof line, matrixInputFile) != NULL) {

		char* token = strtok(line, " ");
		int elementNo = 0;
		while ( token != NULL) {
			if(lineNo == 0) {
				//If the element number is 0; it assigns it to the rows.
				if(elementNo == 0)
					rows = atoi(token);
				//If the element number is 1; it assigns it to the columns.
				if(elementNo == 1) {
					cols = atoi(token);
					//It make the matrix to a specific rows and column size.
					matrix->paint_mat = (char *) realloc(matrix->paint_mat, rows * cols * sizeof(char));

					if(matrix->paint_mat == NULL)
						break;
					//Stores the values of rows and columns into the original struct variables.
					matrix->rows = rows;
					matrix->cols = cols;
					resetMatrix(matrix);
				}
			} else {
				//Stores the elements into the original matirx.
				matrix->paint_mat[((lineNo - 1) * cols) + elementNo] = token[0];
			}

			token = strtok(NULL, " ");
			elementNo++;
		}
		if(matrix->paint_mat == NULL)
			break;
		lineNo++;
	}

	// Close the input file
	fclose(matrixInputFile);
	return matrix;
}

/*Saves the current matrix into a custom file name specified by the user, which can be later used to load it back.
* Arguments:
*	Paint matrix struct.
*	The name of the file.
*
* Returns - Void
*/
void savePaintMatrix(struct paint_matrix *matrix, char *fileName) {
	//File pointer to open the file.
	FILE *paintMatrixFile;
	int i, j;

	//Opens the file.
	paintMatrixFile=fopen(fileName, "w");
	//Prints the rows and columns of the file in the txt file.
	fprintf(paintMatrixFile, "%d %d\n", matrix->rows, matrix->cols);
	//Using for loop it stores the values of the matrix for backup.
	for(i=0; i<matrix->rows; i++) {
		for(j=0;j<matrix->cols;j++) {
			//char *logData;
			//sprintf(logData, "%c", matrix->paint_mat[(i * matrix->cols) + j]);
			fprintf(paintMatrixFile, "%c ", matrix->paint_mat[(i * matrix->cols) + j]);
		}
		fprintf(paintMatrixFile, "\n");
	}
	//Closing the file pointer.
	fclose(paintMatrixFile);
}

/*
 * This function deletes either a row or column from the matrix.
 * Arguments:-
 *	- Struct paint matrix.
 *	- rows and column indicator / flag.
 *	- target to delete the rows and columns.
 *
 * Return Value: Struct.It returns NULL if program fails to re-allocate memory
*/
struct paint_matrix* deletePaintMatrix(struct paint_matrix *matrix, int intrc, int target) {
	//Storing the old rows and columns.
	int oldRows = matrix->rows;
	int oldCols = matrix->cols;
	//Creating a array to copy the data of the original matix.
	char *oldMatrixData = (char *)malloc(matrix->rows * matrix->cols * sizeof(char));
	//Coping the contents of the matrix to the oldMatrixData for backup.
	int i, j;
	for(i=0; i <(matrix->rows * matrix->cols); i++)
		oldMatrixData[i] = matrix->paint_mat[i];

	/*for(i=0; i<oldRows; i++) {
		for(j=0;j<oldCols;j++) {
			printf("OLD Data i:%d j:%d  --> %c\n", i, j, oldMatrixData[(i * oldCols) + j]);
		}
	} */

	//Declaring the new rows and columns variables.
	int newRows = 0;
	int newCols = 0;

	//Assigns the new rows and columns to the new rows and columns.
	//Changes the size of the orignal matrix to the new size using realloc.
	if(intrc == 'r') {
		newRows = oldRows - 1;
		oldRows = target;
		newCols = oldCols;
		matrix->paint_mat = (char *) realloc(matrix->paint_mat, newRows * oldCols * sizeof(char));
	} else {
		newCols = oldCols - 1;
		oldCols = target;
		newRows = oldRows;
		matrix->paint_mat = (char *) realloc(matrix->paint_mat, oldRows * newCols * sizeof(char));
	}

	/* if(matrix->paint_mat != NULL)
	printf("Memory allocated successfully.\n");
	 else
	printf("ERR in Memory reallocation...\n");
	*/

	if(matrix->paint_mat != NULL) {
		//Assigning the new matrix with the old matrix contents.
		//When the row or col is equal to target, it does not store anything in the array.
		//At the end we get a shifted matrix.
		for(i=0; i<newRows; i++) {
			for(j=0;j<newCols;j++) {
				//printf("processing i:%d j:%d  -->", i, j);

				if(intrc == 'r') {
					if(i < target) {
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i * oldCols) + j];
					} else {
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[((i + 1) * oldCols) + j];
					}
				} else {
					if(j < target) {
						//printf(" j < target data: %c\n", oldMatrixData[(i * (newCols - 1)) + j]);
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i * (newCols + 1)) + j];
					} else {
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i* (newCols + 1)) + j + 1];
					}
				}
			}
		}
	}

	//Freeing the old Matix Data.
	free(oldMatrixData);


	//Assigning newCols and newRows to original cols and rows.
	if(matrix->paint_mat != NULL) {
		matrix->cols = newCols;
		matrix->rows = newRows;
	}

	return matrix;
}

/*This function is used to add row or column to a matrix
* Arguments:
*	- Paint struct matrix
*	- row or column indicator
*	- the row location which needs to be added
*
* Return Value: Updated Struct. It returns NULL in case there is failure in re-allocating memory.
*/
struct paint_matrix* addPaintMatrix(struct paint_matrix *matrix, int intrc, int target) {
	//Storing to old number of rows.
	int oldRows = matrix->rows;
	//Storing the old number of columns
	int oldCols = matrix->cols;
	//Creating a matrix to store the contents of currrent matrix.
	char *oldMatrixData = (char *)malloc(matrix->rows * matrix->cols * sizeof(char));
	int i, j;
	//Storing the current matrix data into the old matrix for back up.
	for(i=0; i <(matrix->rows * matrix->cols); i++)
		oldMatrixData[i] = matrix->paint_mat[i];

	/*for(i=0; i<oldRows; i++) {
		for(j=0;j<oldCols;j++) {
			printf("OLD Data i:%d j:%d  --> %c\n", i, j, oldMatrixData[(i * oldCols) + j]);
		}
	} */

	//New rows and column variables declared.
	int newRows = 0;
	int newCols = 0;

	//Assigns the new rows and columns to the new rows and columns.
	//Changes the size of the orignal matrix to the new size using realloc.
	if(intrc == 'r') {
		newRows = oldRows + 1;
		oldRows = target;
		newCols = oldCols;
		matrix->paint_mat = (char *) realloc(matrix->paint_mat, newRows * oldCols * sizeof(char));
	} else {
		newCols = oldCols + 1;
		oldCols = target;
		newRows = oldRows;
		matrix->paint_mat = (char *) realloc(matrix->paint_mat, oldRows * newCols * sizeof(char));
	}


	// if(matrix->paint_mat != NULL)
	// 	printf("Memory allocated successfully.\n");
	// else
	// 	printf("ERR in Memory reallocation...\n");

	if(matrix->paint_mat != NULL) {
		//Assigning the new matrix with the old matrix contents.
		//If the row or column meets the target it hold the '*' value.
		//At the end we get a shifted matrix.
		for(i=0; i<newRows; i++) {
			for(j=0;j<newCols;j++) {
				//printf("processing i:%d j:%d  -->", i, j);

				if(intrc == 'r') {
					if(i < target) {
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i * oldCols) + j];
					} else {
						if( i == target) {
							matrix->paint_mat[(i * newCols) + j] = '*';
						} else {
							matrix->paint_mat[(i * newCols) + j] = oldMatrixData[((i - 1) * oldCols) + j];
						}
					}

				} else {
					if(j < target) {
						//printf(" j < target data: %c\n", oldMatrixData[(i * (newCols - 1)) + j]);
						matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i * (newCols - 1)) + j];
					} else {
						if( j == target) {
							//printf(" j == target \n");
							matrix->paint_mat[(i * newCols) + j] = '*';
						} else {
							//printf(" j > target \n");
							matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i* (newCols - 1)) + j - 1];
						}
					}
				}
			}
		}
	}

	//Free it oldMatrixData.
	free(oldMatrixData);

	//Assign the newcols and new rows to the original cols nad rows.
	if(matrix->paint_mat != NULL) {
		matrix->cols = newCols;
		matrix->rows = newRows;
	}

	return matrix;

}

/*This function re-sizes the array to the new value specified in arguments
* Argument:-
*	- Paint struct matrix.
*	- new number of rows.
*	- new number of cols.
*
* Return Value: Update struct.It returns NULL in case the program failed re-allocating memory
*
*/
struct paint_matrix* resizePaintMatrix(struct paint_matrix *matrix, int newRows, int newCols) {

	//storing the number of ros and cols into oldRows and oldCols.
	int oldRows = matrix->rows;
	int oldCols = matrix->cols;

	//Creating a matrix to store the origninal matirx content.
	char *oldMatrixData = (char *)malloc(matrix->rows * matrix->cols * sizeof(char));
	int i, j;

	//Coping the original matrix content to the new matrix.
	for(i=0; i <(matrix->rows * matrix->cols); i++)
		oldMatrixData[i] = matrix->paint_mat[i];

	//Changing the size of the old matrix.
	matrix->paint_mat = (char *) realloc(matrix->paint_mat, newRows * newCols * sizeof(char));

	/*if(matrix->paint_mat != NULL)
		printf("Memory allocated successfully.\n");
	else
		printf("ERR in Memory reallocation...\n");
	*/
	if(matrix->paint_mat != NULL) {
		//Used to copy the content to the new resized matrix from the old matrix.
		for(i=0; i<newRows; i++) {
			for(j=0;j<newCols;j++) {
				//printf("processing i:%d j:%d  -->", i, j);
				if((i<oldRows) && (j < oldCols)) {
					//printf("THIS IS OLD DATA\n");
					matrix->paint_mat[(i * newCols) + j] = oldMatrixData[(i * oldCols) + j];
				} else {
					//printf("THIS IS NEW DATA\n");
					matrix->paint_mat[(i * newCols) + j] = '*';
				}
			}
		}
	}

	//Freeing the old matrix data.
	free(oldMatrixData);

	//assigning the original columns and rows to the new number of rows and columns.
	if(matrix->paint_mat != NULL) {
		matrix->cols = newCols;
		matrix->rows = newRows;
	}

	return matrix;

}

/*This function erases a cell from the matrix.
*Argument:-
*	- paint struct matrix.
*	- row of cell.
*	- column of cell.
*
* Return Value: void.
*/
void eraseCell(struct paint_matrix *matrix, int row, int col) {
	//If row is less than the original matrix rows along with columns being less than the origninal columns.
	//It ass '*' to the cells in the matrix.
	if((row <= matrix->rows) && (col <= matrix->cols))
		matrix->paint_mat[(row * matrix->cols) + col] = '*';
	else
		print_error(INVALID_CELL_NUMBER);
}

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
void writeLeftDiagonal(struct paint_matrix *matrix, int r1, int r2, int c1, int c2) {
	//Declaring temporary row and temporary column
	int tempR = 0;
	int tempC = 0;

	//If and else to compute the postition of the placement 
	if(c1 > c2) {
		tempR = r1;
		r1 = r2;
		r2 = tempR;

		tempC = c1;
		c1 = c2;
		c2 = tempC;
	}

	//assigning \ to the appropiate location
	if((matrix->paint_mat[(r1 * matrix->cols) + c1] == '*') || (matrix->paint_mat[(r1 * matrix->cols) + c1] == '\\'))
		matrix->paint_mat[(r1 * matrix->cols) + c1] = '\\';
	else
		matrix->paint_mat[(r1 * matrix->cols) + c1] = '+';
	if(c1 == c2) {
		return;
	}
	//Recursively update the matrix of each affected cell
	writeLeftDiagonal(matrix, r1 - 1, r2, c1 +1, c2);
}

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
void writeRightDiagonal(struct paint_matrix *matrix, int r1, int r2, int c1, int c2) {
	//Declaring temporary row and temporary column
	int tempR = 0;
	int tempC = 0;

	//If and else to compute the position of the placement
	if(r1 > r2) {
		tempR = r1;
		r1 = r2;
		r2 = tempR;

		tempC = c1;
		c1 = c2;
		c2 = tempC;
	}

	//assigning \ to the appropriate location
	if((matrix->paint_mat[(r1 * matrix->cols) + c1] == '*') || (matrix->paint_mat[(r1 * matrix->cols) + c1] == '/'))
		matrix->paint_mat[(r1 * matrix->cols) + c1] = '/';
	else
		matrix->paint_mat[(r1 * matrix->cols) + c1] = '+';
	if(r1 == r2) {
		return;
	}
	//Recursively update the matrix of each affected cell
	writeRightDiagonal(matrix, r1 + 1, r2, c1 +1, c2);
}

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
void writeVertical (struct paint_matrix *matrix, int col, int r1, int r2) {
	//Use to store temporary data
	int temp = 0;

	//If and else to compute the postition of the placement 
	if(r1 > r2) {
		temp = r1;
		r1 = r2;
		r2 = temp;
	}
	//assigning | to the appropiate location
	if((matrix->paint_mat[(r1 * matrix->cols) + col] == '*') || (matrix->paint_mat[(r1 * matrix->cols) + col] == '|'))
		matrix->paint_mat[(r1 * matrix->cols) + col] = '|';
	else
		matrix->paint_mat[(r1 * matrix->cols) + col] = '+';
	if(r1 == r2)
		return;
	
	//Recursively update the matrix of each affected cell
	writeVertical(matrix, col, r1 +1 , r2);

}

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
void writeHorizontal (struct paint_matrix *matrix, int row, int c1, int c2) {
	//Use to store temporary data
	int temp = 0;
	if(c1 > c2) {
		temp = c1;
		c1 = c2;
		c2 = temp;
	}

	//If and else to compute the postition of the placement 
	if((matrix->paint_mat[(row * matrix->cols) + c1] == '*') || (matrix->paint_mat[(row * matrix->cols) + c1] == '-'))
		matrix->paint_mat[(row * matrix->cols) + c1] = '-';
	else
		matrix->paint_mat[(row * matrix->cols) + c1] = '+';
	if(c1 == c2)
		return;

	//Recursively update the matrix of each affected cell
	writeHorizontal(matrix, row, c1 +1 , c2);

}

/*This function is used to identify which direction of updating the paint matrix variable.
* Arguments:-
*	- row co-ordinate 1
*	- row co-ordinate 2
*	- column co-ordinate 1
*	- column co-ordinate 2
*
* Return Value: enum of direction
*/
enum paint_direction getWriteDirection(int x1, int y1, int x2, int y2) {

	// printf ("x1 :%d:   x2 :%d:    y1 :%d:     y2 :%d: \n", x1, x2, y1, y2);

	if(x2 == x1) {
		// printf("paint direction is HORIZONTAL\n");
		return HORIZONTAL;
	}
	if(y2 == y1) {
		// printf("paint direction is VERTICAL\n");
		return VERTICAL;
	}

	//finding the slope to calculate if it is a Left or Right diagonal
	float slope = (float) (y2-y1) / (x2-x1);

	//printf("slope is :%f:\n", slope);

	if(slope == 1) {
		// printf("paint direction is RIGHT_DIAGONAL\n")
		return RIGHT_DIAGONAL;
	}
	if(slope == -1) {
		// printf("paint direction is LEFT_DIAGONAL\n");
		return LEFT_DIAGONAL;
	}

	return ERROR;
}

// end of file
