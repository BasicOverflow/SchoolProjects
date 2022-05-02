#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "square_matrix.h"

///////////////////////////////////////////////////////////////////////

// Define any necessary macros, types, and additional functions here

typedef struct {
    long start_row_i;
    long end_row_i;
    long num_cols;
    long double* sq_sum; // space that holds some of squares for each col 
    matrix_element** data;
} thread_info;

void* matrixNormChunk(void* arg) {
    thread_info* thread_arg = (thread_info*)arg;
    // copy arg fields to local variables
    long n = thread_arg->num_cols;
    matrix_element** dataL = thread_arg->data;

    for (long i=thread_arg->start_row_i; i<thread_arg->end_row_i; i++) {
        for (long j=0; j<n; j++) {
            thread_arg->sq_sum[j] += dataL[i][j] * dataL[i][j]; 
        }
    }

    pthread_exit(NULL);
}


///////////////////////////////////////////////////////////////////////

/*
 * Compute and return the L2,1 norm of matrix m using multi-threading.
 *
 * Return NAN if anything is wrong.
 *
 *
 */


long double matrixNorm_threads(square_matrix* m, size_t num_threads)
{
    if(m==NULL) { return NAN; }
    
    size_t n = m->order;
    matrix_element** data = m->data;
    long double* sq_sum = calloc(n, sizeof(long double)); 

    // init thread stuff
    pthread_t threads[num_threads];
    thread_info args[num_threads];

    // create threads, fill thread args
    for (int i=0; i<num_threads; i++) {
        long start = i*n/num_threads; // first element in i-th chunk
        long end   = (i==num_threads-1 ? n : (i+1)*n/num_threads ); // first element *after* i-th chunk
        args[i].start_row_i =  start;
        args[i].end_row_i = end;
        args[i].num_cols = n;
        args[i].data = data;
        args[i].sq_sum = sq_sum;

        // printf("Thread #%d in charge of rows %ld->%ld\n", i, start, end);

        pthread_create(&threads[i], NULL, matrixNormChunk, (void*)&args[i]);  
    }

    // join threads
    for (int i=0; i<num_threads; i++) { pthread_join(threads[i], NULL); }
    
    // compute grand result
    long double norm = 0.0;
    for (size_t j=0; j<n; j++) {
        norm += sqrt(sq_sum[j]);
    }

    free(sq_sum);
    return norm;
}
