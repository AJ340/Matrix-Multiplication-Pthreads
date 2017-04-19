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
#include "matrix.h"

matrix a, b, c;

// Struct for data being sent to each thread
typedef struct thread_data_struct
{
	int i;
	int j;
} thread_data;

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