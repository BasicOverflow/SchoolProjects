#include <pthread.h>

// Compute dot product of two n-dimensional vectors

typedef struct {
   long id;
   long len;
   long num_threads;
   long* loc_a;
   long* loc_b;
   long loc_dotprod;
} thread_info;


void* dotprod(void* arg)
{
   thread_info* my_data = arg;
   long result = 0;

   for(long i = my_data->id; i < my_data->len; i += my_data->num_threads) {
      result += my_data->loc_a[i] * my_data->loc_b[i];
   }
   my_data->loc_dotprod = result;
   pthread_exit(NULL);
}


long dotprod_threads(long* a, long* b, long len, int num_threads)
{
   pthread_t thread[num_threads];
   thread_info args[num_threads];

   long dotproduct=0; // to store result

   // create threads and pass them the thread index
   for(long i=0; i<num_threads; i++) {
      // populate struct for thread to do work in
      args[i].id = i;
      args[i].len = len;
      args[i].num_threads = num_threads;
      args[i].loc_a = a;
      args[i].loc_b = b;
      args[i].loc_dotprod = 0;

      pthread_create(&thread[i], NULL, dotprod, &args[i]);
   }

   // join threads
   for(long i=0; i<num_threads; i++) {
      pthread_join(thread[i], NULL);
      dotproduct += args[i].loc_dotprod;
   }
   return dotproduct;
}

