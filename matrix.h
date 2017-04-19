// MATRIX.H
#ifndef MATRIX_H
#define MATRIX_H

	// Access arrays as if they are 2D
	#define array(arr, num_cols, i, j) arr[(int) num_cols * (int) i + (int) j]

	// Struct for matricies
	typedef struct matrix_struct 
	{
		int rows;
		int cols;
		int * data;
	} matrix;

	// Initialize the matrix size and allocate memory for it
	void initialize_matrix(matrix *m, int r, int c);

	// Check to see if matric has memory allocated and the number of rows and columns is a positive integer
	int verify_matrix(matrix *to_verify);

	// Read a cell in matrix
	int read_matrix_cell(matrix *to_read, int i, int j);

	//Write to a cell in matrix
	void write_matrix_cell(matrix *to_write, int i, int j, int new_val);

	// Fill matrix with random integers from 1 to 10
	int fill_matrix(matrix *to_fill);

	// Print entire matrix
	void print_matrix(matrix *to_print);

	// END OF MATRIX FUNCTIONS
	
#endif //MATRIX.H
