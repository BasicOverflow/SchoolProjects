#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "unixtimer.h"

#define DEFAULT_N  1000000
#define DEFAULT_NTHREADS 2

//
// Compute the sum of first n terms of the Madhava-Leibniz series
// using num_threads threads. Timing macros omitted
//


typedef struct {
   long start_n;
   long end_n;
   double chunk_result;
} thread_info;


void* compute_chunk(void* arg) {
   thread_info* my_data = arg;
   double result = 0;
   
   for (long i=my_data->start_n; i<my_data->end_n; i++) {
      if(i%2==0)
         result += 1.0/(2*i+1);
      else
         result -= 1.0/(2*i+1);
   }

   my_data-> chunk_result = result;
   pthread_exit(NULL);
}


double sum_threads(long n, int num_threads)
{
   // TODO: compute [0->n] terms of series 
   pthread_t threads[num_threads];
   thread_info args[num_threads];

   // create threads
   for (int i=0; i<num_threads; i++) {
      long start = i*n/num_threads; // first element in i-th chunk
      long end   = (i==num_threads-1 ? n : (i+1)*n/num_threads ); // first element *after* i-th chunk
      args[i] = (thread_info){start, end};

      pthread_create(&threads[i], NULL, compute_chunk, (void*)&args[i]);
   }

   // join threads
   double final_res = 0;
   for (int i=0; i<num_threads; i++) {
      pthread_join(threads[i], NULL);
      final_res += args[i].chunk_result;
   }
   return final_res;
}

/////////////////////////////////////
// PROVIDED SKELETON CODE BELOW

int main(int argc, char *argv[])
{
   long n           = (argc < 2 ? DEFAULT_N : atoi(argv[1]) );
   int  num_threads = (argc < 3 ? DEFAULT_NTHREADS : atoi(argv[2]) );
   double PI25 = 3.141592653589793238462643;

   // Compute and print the approximation of pi
   start_clock();
   start_timer();
   double my_pi = 4 * sum_threads(n, num_threads);
   printf("pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - PI25));
   printf ("Clock time = %.2f CPU time =  %.2f\n", clock_seconds(), cpu_seconds() );

   return 0;
}
