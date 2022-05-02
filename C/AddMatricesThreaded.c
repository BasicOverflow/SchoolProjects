#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "square_matrix.h"
#include <stdlib.h>

// Sum to given matricies in a multi-threaded fashion


///////////////////////////////////////////////////////////////////////

// Define any necessary macros, types, and additional functions here
typedef int matrix_element;

typedef struct {
    size_t order;
    matrix_element** data;
} square_matrix;

typedef struct {
   int num_cols;
   int num_rows;
   int start_row_i;
   int end_row_i;
   matrix_element** m1_rows; // given rows to compute from 1st matrix
   matrix_element** m2_rows; // given rows to compute from 2nd matrix
   square_matrix* res_m; // where to place summed rows
} thread_info;


void* sum_rows(void* arg) {
   thread_info* thread_arg = (thread_info*)arg;
   // copy field args into local variables
   int n = thread_arg -> num_cols;
   matrix_element** m1_rows_loc = thread_arg -> m1_rows;
   matrix_element** m2_rows_loc = thread_arg -> m2_rows;
   matrix_element** result_rows_loc = thread_arg -> res_m -> data;

   // compute result
   for(int row = thread_arg -> start_row_i; row < thread_arg -> end_row_i; row++) {
      for(int col = 0; col<n; col++) {
         result_rows_loc[row][col] = m1_rows_loc[row][col] + m2_rows_loc[row][col];  
      }
   }
   // terminate
   pthread_exit(NULL);
}


/*
 * Compute the sum of two square matrices. Return a pointer to the
 * newly allocated result matrix or NULL if anything is wrong
 *
 * Similar to add_square_matrices() in square_matrix.c but using multi-threading
 */
square_matrix* add_square_matrices_threads(square_matrix *m1, square_matrix *m2, size_t num_threads)
{
   // TODO
   if(m1 == NULL || m2 == NULL || m1->order != m2->order)
      return NULL;

   int n = (int)m1->order;
   // printf("Num rows: %d\n", (int)n);
   // row of matrix_element* pointers 
   // each matrix_element* holds a column of matrix_elements (ints)
   matrix_element** data1 = m1->data;
   matrix_element** data2 = m2->data;

   // make n threads, assign a chunk of rows from both matricies to each thread
   // each thread computes a chunk of resulting rows

   // create threads
   pthread_t threads[num_threads]; // holds thread id's
   thread_info args[num_threads]; // holds argument structs for each thread

   // create resulting matrix
   square_matrix* res = new_square_matrix(n);
   if(res == NULL) return NULL;
   
   for (int i=0; i<num_threads; i++) {
      // find the chunk of rows from rach matrix to give to each thread
      int start = i*n/num_threads; // first element in i-th chunk                              
      int end = (i==num_threads-1 ? n : (i+1)*n/num_threads ); // first element *after* i-th chunk 
 
      // printf("Thread #%d in charge of rows %d->%d\n", i, start, end);

      // fill in arg field for current thread
      args[i].start_row_i = start;
      args[i].end_row_i = end;
      args[i].num_cols = n;
      args[i].res_m = res;

      int j = 0; // keeps track of indexing for m1_row/m2_row fields
      for (int curr_row=start; curr_row<end; curr_row++,j++ ) {
         args[i].m1_rows = data1;
         args[i].m2_rows = data2;
      }
      // initialize thread, pass in its struct
      pthread_create(&threads[i], NULL, sum_rows, (void*)&args[i]);
   }

   // join threads
    for (int i=0; i<num_threads; i++) {
       pthread_join(threads[i], NULL); 
    }
    return res;
}


