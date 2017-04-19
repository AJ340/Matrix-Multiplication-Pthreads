// Andres Quinones
// Unix Tools Spring 2017
// Homework 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

// Access arrays as if they are 2D
#define array(arr, num_cols, i, j) arr[(int) num_cols * (int) i + (int) j]

// Struct for matricies
typedef struct matrix_struct
{
	int rows;
	int cols;
	int *data;
} matrix;

// Create our 3 matrices for multiplication
// Global so all threads can access
matrix a, b, c;

// Struct for data being sent to each thread
typedef struct thread_data_struct
{
	int i;
	int j;
} thread_data;

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

// Our routine for multiply. Done on a per cell basis in output matrix
void * multiply(void *arg)
{
	// Extract data from arg
	thread_data * t_arg = (thread_data *) arg; 
	int i = t_arg->i;
	int j = t_arg->j;
	//printf("i:%d j:%d\n", i, j);

	// Compute value for cell
	int value = 0;
	for (int a_j = 0; a_j < a.cols; a_j++)
	{
		int one = read_matrix_cell(&a, i, a_j);
		int two = read_matrix_cell(&b, a_j, j);
		//printf("One:%d , Two:%d\n", one, two);
		value = value + (one * two); 
	}
	// Write value into output matrix
	write_matrix_cell(&c, i, j, value);
	pthread_exit(0);
}


int main(int argc, char const *argv[])
{
	/* code */
	// Seed random with time in microseconds
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	// Matricies must be initialized properly or fill will fail.
	// A columns and B rows must be equal
	// C must be of size A_Rows x B_Cols
	initialize_matrix(&a, 5, 4);
	initialize_matrix(&b, 4, 5);
	initialize_matrix(&c, 5, 5);

	// Fill Input Matricies with random values
	printf("Fill A returned: %d\n", fill_matrix(&a));
	printf("Fill B returned: %d\n", fill_matrix(&b));

	// Print A
	printf("Matrix A:\n");
	print_matrix(&a);

	// Print B
	printf("\nMatrix B:\n");
	print_matrix(&b);

	// Make dynamic array of threadIDs
	pthread_t * threads = malloc(sizeof(pthread_t) * (c.rows * c.cols));

	// Make array for thread_data. 
	// We will be making a thread for each cell in the output matrix
	// So there are that many thread_data structs 
	thread_data * all_t_args = malloc(sizeof(thread_data) * (c.rows * c.cols));

	// For each cell in c ...
	for (int i=0; i < c.rows; i++)
	{
		for (int j=0; j < c.cols; j++)
		{
			// Store data needed for thread
			array(all_t_args, c.cols, i, j).i = i;
			array(all_t_args, c.cols, i, j).j = j;
			// Create thread and have it start executing from multiply.
			pthread_create(&array(threads, c.cols, i, j), NULL, multiply, (void *) &all_t_args[i*c.rows+j]);
		}
	}

	// Join all the threads
	for (int i=0; i < c.rows; i++)
	{
		for (int j=0; j < c.cols; j++)
		{
			pthread_join(array(threads, c.cols, i, j),NULL);
		}
	}

	// Print computed C matrix after threads finish
	printf("\nMatrix C:\n");
	print_matrix(&c);

	// Free dynamic memory
	if (threads != NULL)
		free(threads);
	if (all_t_args != NULL)
		free(all_t_args);
	free(a.data);
	free(b.data);
	free(c.data);

	return 0;
}