// MATRIX.C

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

// Access arrays as if they are 2D
#define array(arr, num_cols, i, j) arr[(int) num_cols * (int) i + (int) j]

// Initialize the matrix size and allocate memory for it
void initialize_matrix(matrix *m, int r, int c)
{
	m->rows = r;
	m->cols = c;
	m->data = (int *) malloc(sizeof(int) * (m->rows * m->cols));
}

// Check to see if matric has memory allocated and the number of rows and columns is a positive integer
int verify_matrix(matrix *to_verify)
{
	if (to_verify->data == NULL || to_verify->rows <= 0 || to_verify->cols <= 0)
		return 0;
	return 1;
}

// Read a cell in matrix
int read_matrix_cell(matrix *to_read, int i, int j)
{
	return array(to_read->data, to_read->cols, i, j);
}

//Write to a cell in matrix
void write_matrix_cell(matrix *to_write, int i, int j, int new_val)
{
	array(to_write->data, to_write->cols, i, j) = new_val;
}

// Fill matrix with random integers from 1 to 10
int fill_matrix(matrix *to_fill)
{
	if(!verify_matrix(to_fill))
		return 0;

	for (int i=0; i < to_fill->rows; i++)
	{
		for (int j=0; j < to_fill->cols; j++)
		{
			if (to_fill->data == NULL)
			{
				to_fill->data = (int *) malloc(sizeof(int) * (to_fill->rows * to_fill->cols));
			}
			int value = rand() % 10 + 1;
			write_matrix_cell(to_fill, i, j, value);
		}
	}
	return 1;
}

// Print entire matrix
void print_matrix(matrix *to_print)
{
	for (int i=0; i < to_print->rows; i++)
	{
		for (int j=0; j < to_print->cols; j++)
		{
			int cell_value = read_matrix_cell(to_print, i, j);
			printf("%d ", cell_value);
		}
		printf("\n");
	}
}
// END OF MATRIX FUNCTIONS