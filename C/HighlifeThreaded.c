#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

// Highlife is a cellular automata based on Conway's famouns Game of Life.
// Assignment: Given a sequential implementation of the simulation, 
// upgrade to a multi-threaded version in order to increase performance,
// ultilizing thread synchronization primitives. Visualization included.

#define DEFAULT_FILE_NAME    "start0.txt"
#define DEFAULT_CYCLES       100
#define DEFAULT_PRINT_FLAG   1
#define DEFAULT_NUM_THREADS  2

#define REFRESH_MILLISEC 1000/30

// define barrier 
static pthread_barrier_t barrier;

void msleep(int tms)
{
    struct timeval tv;
    tv.tv_sec  = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    select (0, NULL, NULL, NULL, &tv);
}

typedef struct {
   int    row;
   int    col;
   char** src;
} Board;

typedef struct {
   int numCycles;
   int row_start_i;
   int row_end_i;
   int printFlag;
   int cols;
   Board* board1;
   Board* board2;
} thread_info;


Board* makeBoard(int r, int c)
{
   Board* p = malloc(sizeof(Board));
   p->row   = r;
   p->col   = c;
   p->src   = malloc(sizeof(char*)*r);
   for(int i=0; i<r; i++)
      p->src[i] = malloc(sizeof(char)*c);
   return p;
}

void freeBoard(Board* b)
{
   for(int i=0; i<b->row; i++)
      free(b->src[i]);
   free(b->src);
   free(b);
}

Board* readBoard(char* fName)
{
   int row, col;
   FILE* src = fopen(fName,"r");
   fscanf(src, "%d %d\n", &row, &col);
   Board* rv = makeBoard(row, col);
   for(int i=0; i<row; i++) {
      for(int j=0; j<col; j++) {
         char ch = fgetc(src);
         rv->src[i][j] = ch == '*';
      }
      char skip = fgetc(src);
      while (skip != '\n') skip = fgetc(src);
   }
   fclose(src);
   return rv;
}

void saveBoard(Board* b, FILE* fd)
{
   for(int i=0; i<b->row; i++) {
      fprintf(fd,"|");
      for(int j=0; j < b->col; j++)
         fprintf(fd,"%c",b->src[i][j] ? '*' : ' ');
      fprintf(fd,"|\n");
   }
}

void printBoard(Board* b)
{
   printf("\033\143");  /* this is to clear the terminal */
   saveBoard(b,stdout);
}

int liveNeighbors(int i, int j, Board* b)
{
   const int pc = (j-1) < 0 ? b->col-1 : j - 1;
   const int nc = (j + 1) % b->col;
   const int pr = (i-1) < 0 ? b->row-1 : i - 1;
   const int nr = (i + 1) % b->row;
   int xd[8] = {pc , j , nc, pc, nc, pc , j , nc };
   int yd[8] = {pr , pr, pr, i , i , nr , nr ,nr };
   int ttl = 0;
   for(int k=0; k < 8; k++)
      ttl += b->src[yd[k]][xd[k]];
   return ttl;
}

void evolveBoard(Board* in, Board* out)
{
   static int rule[2][9] = {
      {0,0,0,1,0,0,1,0,0},
      {0,0,1,1,0,0,0,0,0}
   };
   for(int i=0; i < in->row; i++) {
      for(int j=0; j < in->col; j++) {
         int ln = liveNeighbors(i, j, in);
         int c  = in->src[i][j];
         out->src[i][j] = rule[c][ln];
      }
   }
}

void* evolveBoardChunk(void* arg)
{
   static int rule[2][9] = {
      {0,0,0,1,0,0,1,0,0},
      {0,0,1,1,0,0,0,0,0}
   };
   thread_info* thread_arg = (thread_info*)arg;
   // copy field args into local variables
   Board* b[2] = { thread_arg->board1, thread_arg->board2 };
   int printFlag = thread_arg -> printFlag;
   int start_row = thread_arg -> row_start_i;
   int end_row = thread_arg -> row_end_i;
   int cols = thread_arg -> cols;
   // current in=b[g%2] | current out=b[1-g%2]

   for (int g=0; g<thread_arg->numCycles; g++) {
      if (printFlag) { printBoard(b[g%2]); msleep(REFRESH_MILLISEC); }
      // evolve board:  
      for(int i=start_row; i < end_row; i++) {
         for(int j=0; j < cols; j++) {
            int ln = liveNeighbors(i, j, b[g%2]);
            int c  = b[g%2]->src[i][j];
            b[1-g%2]->src[i][j] = rule[c][ln];
         }
      }
      // barrier wait for rest of threads    
      pthread_barrier_wait(&barrier);
   }
   // terminate
   pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
   char* file_name  = (argc < 2 ? DEFAULT_FILE_NAME : argv[1] );
   int cycles      = (argc < 3 ? DEFAULT_CYCLES : atoi(argv[2]) );
   int printFlag   = (argc < 4 ? DEFAULT_PRINT_FLAG : atoi(argv[3]) );
   int num_threads = (argc < 5 ? DEFAULT_NUM_THREADS : atoi(argv[4]) );

   if (argc < 2) {
       fprintf(stderr, "Usage: %s <start_file> <cycles> <print_flag> <num_threads>\n", argv[0]);
       exit(1);
   }

   Board* board1 = readBoard(file_name);
   Board* board2 = makeBoard(board1->row, board1->col);
   Board* b[2] = {board1, board2};

   // init thread stuff
   pthread_t threads[num_threads];
   thread_info args[num_threads];
   int rows = board1->row;

   // init barier sutff
   int s = pthread_barrier_init(&barrier, NULL, num_threads);
   if (s != 0) { printf("Barrier init failed\n"); }

   // loop through num_threads, create each thread and pass in its chunk of rows, and each thread will loop through all cycles
   for (int i=0; i<num_threads; i++) {
      int start = i*rows/num_threads; // first element in i-th chunk                              
      int end = (i==num_threads-1 ? rows : (i+1)*rows/num_threads ); // first element *after* i-th chunk 
      // printf("Thread #%d in charge of rows %d->%d\n", i, start, end);

      // fill in arg fields for given thread
      args[i].numCycles = cycles;
      args[i].row_start_i = start;
      args[i].row_end_i = end;
      args[i].cols = board1->col;
      args[i].printFlag = printFlag;
      args[i].board1 = b[0];
      args[i].board2 = b[1];

      // create threads
      pthread_create(&threads[i], NULL, evolveBoardChunk, (void*)&args[i]);
   }   

   // join threads
   for (int i=0; i<num_threads; i++) {
      pthread_join(threads[i], NULL); 
   }

   if (printFlag)
       printBoard(b[1-cycles%2]);

   FILE* final = fopen("final_threads.txt","w");
   saveBoard(b[1-cycles%2], final);
   fclose(final);
   freeBoard(board1);
   freeBoard(board2);
   pthread_barrier_destroy(&barrier);
   return 0;
}



// Plan: 
   // split up work done by rows
      // -i.e, make each thread compute n/total rows of the board
   // Worker function:
      // has for loop that iterates how many generations there are
      // upon each iteration, worker computes is given chunk of rows, than uses 
      // barrier to wait, repeat for n cycles
      // keep same structure as evolve_barrier, just also pass in wich rows to only compute 
