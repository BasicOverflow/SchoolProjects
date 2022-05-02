#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define     DEFAULT_DARTS_PER_THREAD 500000
#define     DEFAULT_NUM_THREADS           2

// Program that utilizes the monte-carlo 'dart-throwing' algorithm to 
// approximate the value of PI with extreme precision
// Macros for creating the 'random darts' have been omitted

typedef struct {
    int num_darts;
    int p_loc;
} thread_info;


void* throw_darts(void* arg) {
    thread_info* my_data = arg;
    RANDOM_INIT();
    int res = 0;
    for (int dart=0; dart<my_data->num_darts; dart++) {
        double x = RANDOM_DOUBLE();
        double y = RANDOM_DOUBLE();
        if ( (x*x) + (y*y) < 1 ) { res++; }
    }
    my_data->p_loc = res;
    pthread_exit(NULL);
}


int main(int argc, char ** argv)
{
    int     num_threads      = DEFAULT_NUM_THREADS;
    int     darts_per_thread = DEFAULT_DARTS_PER_THREAD;

    if (argc > 2) {
        num_threads = atoi(argv[1]);
        darts_per_thread = atoi(argv[2]);
        assert( num_threads>0 && darts_per_thread>0 );
    }
    else {
        fprintf(stderr, "Usage: %s <num_threads> <darts_per_thread>\n", argv[0]);
        fprintf(stderr, "Currently using the default values\n");
    }

    long long n = (long long)num_threads * darts_per_thread;

    printf("Throwing %lld darts using %d thread%s\n", n, num_threads, (num_threads>1 ? "s" : "") );

    // Add code to create num_threads threads, each simulating the throwing of
    // darts_per_thread darts at the [0, 1) × [0, 1) square.  Compute in p the
    // number of darts that fall within the circle of radius 1 centered at (0,0)

    pthread_t threads[num_threads]; // array for thread IDs
    thread_info args[num_threads]; // holds argument structs for each thread

    // create threads
    for (int i=0; i<num_threads; i++) {
        args[i].num_darts = darts_per_thread;
        args[i].p_loc = 0; // initialized to 0
        pthread_create(&threads[i], NULL, throw_darts, (void*)&args[i]);
    }

    // join threads
    int p = 0;
    for (int i=0; i<num_threads; i++) {
        pthread_join(threads[i], NULL);
        p += args[i].p_loc;
    }

    // estimate pi and compare with M_PI, the value defined in math.h
    double my_pi = (double)p / n * 4.0;
    printf("Pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - M_PI));
    return 0;
}


